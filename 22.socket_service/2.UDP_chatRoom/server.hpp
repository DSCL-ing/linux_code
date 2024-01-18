#pragma once

#include<iostream>
#include<string>
#include<functional>
#include<unordered_map>
#include<mutex>
#include<vector>
#include<thread>


#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#include<cerrno>
#include<cstring>
#include<cstdlib>

#include"err.hpp"
#include"ringQueue.hpp"

static const uint16_t default_port = 8080;

class UdpServer
{
public:
  UdpServer(uint16_t port = default_port):port_(port)
  {
      std::cout<<" , server port: " << port <<std::endl;
      
      //std::function<void()> func = std::bind(&UdpServer::Recv,this);
      //std::thread p(std::bind(&UdpServer::Recv,this));
      //作用:回忆bind用法
  }
  ~UdpServer()
  {
    p->join();
    c->join();

    delete p;
    delete c;
  }

  void start()
  {
    //1. 打开socket接口,打开网络文件
    sock_ = socket(AF_INET,SOCK_DGRAM,0);
    if(sock_<0)
    {
      std::cout<<"create socket error :"<<strerror(errno)<<std::endl;
      exit(SOCKET_ERR);
    }
    std::cout<<"create socket success: " <<sock_<<std::endl;

    //1.5 现在本地给本地套接字字段指明IP和端口号Port 
    
    struct sockaddr_in local;
    memset(&local,0,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port_);//主机转网络,16位短整数
    
    //2.将准备好的本地套接字字段和网络文件套接字字段绑定
    int n = bind(sock_,(struct sockaddr*)&local,sizeof(local)); //sockaddr_in和sockaddr和sockaddr_un等大小是不一样的,所以把类型大小作参数传进来
    if(n < 0 )
    {
      std::cerr<<"bind socker error :"<<strerror(errno)<<std::endl;
      exit(BIND_ERR);
    }
    std::cout<<"bind socket success" << std::endl;


    p = new std::thread(std::bind(&UdpServer::Recv,this)) ;
    c = new std::thread(std::bind(&UdpServer::broadcast,this)) ;

  }
 
  void addUser(const std::string &name,const sockaddr_in &peer) //C++ struct可以不写
  {
    //好像没什么必要写addUser,当作复习接口.实际用[]就可以了.实现删除接口意义更大
    //onlineUser_[name] = peer;
    
    std::lock_guard<std::mutex> lock_guard(mtx_);
    
    auto it = onlineUser_.find(name);
    if(it==onlineUser_.end())
    {
      //onlineUser_.emplace(name,peer);//可变参数包+定位new推导,无需类型,
      //onlineUser_.insert(std::pair<std::string,sockaddr_in>(name,peer));//要写类型
      onlineUser_.insert(std::make_pair(name,peer));//无需写类型
      std::cout<<"insert new user"<<std::endl;
    }
    else
    {
        //存在就什么都不做
    }
    
    
  }

  void Recv()
  {


    char buffer[2048];
    while(true)
    {
      //1.接收
      struct sockaddr_in peer; //本地sockaddr,用于接收套接字地址
      socklen_t len = sizeof(peer);
      int  n = recvfrom(sock_,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&peer,&len); 
      if(n>0)
      { 
        buffer[n] = '\0'; 
      }
      else
      {
        continue; 
      }
     

      std::string clientip = inet_ntoa(peer.sin_addr);
      uint16_t clientport = ntohs(peer.sin_port);
      std::cout<<clientip << "-" <<clientport<<"# " <<buffer<<std::endl;
      
      //添加用户
      std::string username;
      username += clientip;
      username +="-";
      username += std::to_string(clientport);

      addUser(username,peer);

      //一定要先添加用户,再push,否则
      //1.client多线程下会消息错位.
      //2.client单线程阻塞:会出现service在recv,client也在recv
      //原因:broadcast是先pop,再判断用户再发送
      //如果先push,则broadcast可能会立刻pop,此时还没addUser,导致没有发送出去就进入下个循环.发生运行逻辑错误

      //添加任务/数据
      std::string massage;
      massage+=username;
      massage+=": ";
      massage+=buffer;
      rq_.push(massage);

     }  
  }

  //广播给所有用户
  void broadcast()
  {
    while(true)
    {
      usleep(100);
      std::string response;
      rq_.pop(&response);
      
      //加锁方案一:
      //缺点:要加锁的区域涉及sendto系统调用,系统IO等.影响效率
      //std::lock_guard<std::mutex> lock_guard(mtx_);
      //for(auto &v:onlineUser_)  //向每个用户都发送一条消息
      //{
      //  std::cout<<"broadcast massage to "<<v.first<<" : "<<response<<std::endl;
      //  sendto(sock_,response.c_str(),response.size(),0,(struct sockaddr*)&v.second,sizeof(v)); 
      //}
    
      //加锁方案二:
      //优点:内存级的拷贝,没有调用任何系统接口,速度快
      std::vector<sockaddr_in> temp; 
      {
        //std::lock_guard<std::mutex> lock_guard(mtx_);
        for(auto e:onlineUser_)
        {
          temp.push_back(e.second);
        }
      }
      
      for(auto v:temp)
      {
        //std::cout<<"broadcast massage to "<<v.first<<" : "<<response<<std::endl;
        sendto(sock_,response.c_str(),response.size(),0,(struct sockaddr*)&v,sizeof(v)); 
      }
   
    }

  }


private:
  int sock_;  //udp服务器自己的套接字
  uint16_t port_; //udp服务器自己的端口号
  
  RingQueue<std::string> rq_; //用于存放消息
  
  std::unordered_map<std::string,sockaddr_in> onlineUser_;//在线用户
  std::mutex mtx_; //阻止onlineUser_ Map在使用时被修改
  
  std::thread *p;
  std::thread *c;

  //一般输入的IP都是点分十进制的.所以是字符串
  //std::string ip_; //服务器自己的IP --------------------- 云服务器版本不需要手动提供IP地址
}; // end  UdpServer









