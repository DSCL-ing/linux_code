#pragma once
#include <iostream>
#include "Sock.hpp"
#include"Epoller.hpp"
#include<cassert>
#include<functional>
#include<unordered_map>

/*
   一.服务器基本结构
   初始化+启动  //test
   */

const static uint16_t defaultport = 8888;

/*业务逻辑*/
using func_t = std::function<std::string(std::string)>;


/* 每个fd都需要有自己的缓冲区 */
class Connection //将fd和缓冲区联系
{
  public:
    Connection(int fd,std::string clientip ,
        uint16_t clientport )
      :fd_(fd),clientip_(clientip),clientport_(clientport)
    {} 
    ~Connection();

    int fd_;
    std::string inbuffer_;
    std::string outbuffer_;

    //client info , only debug
    std::string clientip_;
    uint16_t clientport_;
};

class EpollServer
{
  const static int g_num = 128;
  public:
  EpollServer(func_t func,uint16_t port = defaultport)
    :port_(port),func_(func)
  {
  }
  void InitServer()
  {
    listensock_.Socket();
    listensock_.Bind(port_);
    listensock_.Listen();
    epoller.Create();

    /*.要为listensock创建connect对象,符合每个fd都有自己的缓冲区,虽然listensock不需要,但统一规则能减少编码成本*/
    AddConnection(listensock_.Fd(),EPOLLIN);

    logMessage(INFO,"init server success");
  }
  void Start()
  {
    while(true)
    {
      //sleep(1);

      //获取服务
      int timeout = -1;
      while(true)
      {
        LoopOnce(timeout);
      }
    }

  }

  /* wait一次,然后处理这一批fd */
  void LoopOnce(int timeout) //原来的HandlerEvents(int num)
  {
    int n = epoller.Wait(revs_,g_num,timeout);
    for(int i = 0; i<n ;i++) //只有n大于0(有就绪事件)才会处理
    {
      sleep(1);

      int fd = revs_[i].data.fd;
      uint32_t events = revs_[i].events;

      logMessage(INFO,"当前正在处理%d上的%s",fd,EPOLLIN&events?"EPOLLIN":"OTHER");

      //读事件就绪
      if(events & EPOLLIN) 
      {
        //EPOLLIN分为新连接就绪和数据可以读取事件

        if(fd == listensock_.Fd()) //取走新连接
          Accepter();
        else //取数据
          Recver(fd);
      }
      else
      {

      }
    }

  }
  
  /*将添加管理操作封装*/
  void AddConnection(int fd,uint32_t events,std::string clientip = "127.0.0.1" ,uint16_t clientport = defaultport)
  {
    Connection* conn = new Connection(fd,clientip,clientport); 
    connections_.insert(std::pair<int,Connection*>(fd,conn));
    bool r = epoller.AddEvent(fd,events); //r == ret
    assert(r);
    (void)r;

    logMessage(INFO,"AddConnection succes, fd: %d,\
       clientinfo:[%s,%d]",fd,clientip.c_str(),clientport);
  }

  void Accepter()
  {

    std::string clientip; 
    uint16_t clientport;
    int sock = listensock_.Accept(&clientip,&clientport);
    //logMessage(INFO,"get a new link...");
    if(sock < 0)
    {
      return ;
    }
    logMessage(INFO,"%s:%d已经连上服务器",clientip.c_str(),clientport);
    
    AddConnection(sock,EPOLLIN,clientip,clientport);
  }

  void Recver(int fd)
  {

    //目前无法保证读到一个完整的报文
    //完整报文应由应用层协议规定.
    char request[1024];
    ssize_t s = recv(fd,request,sizeof(request)-1,0); //给缓冲区最后留个空位,防止放不下\0
    if(s<0)
    {
      logMessage(WARNING,"recv error,client quit");
      epoller.DelEvent(fd);
      close(fd);
    }
    else if(s == 0)
    {
      //返回值为0一般表示读到末尾
      logMessage(INFO,"client quit...");
    }
    else
    {
      request[s] = 0; //给末尾加上\0
      /*telnet的行分隔符是\r\n,仅仅n-1打印会覆盖*/
      /*验证:*/
      //buffer[s-1] = 0;
      //buffer[s-2] = 0;
      //std::cout<<"client# "<<request<<std::endl;
      
      connections_[fd]->inbuffer_+=request;

      /*echo server*/
      //std::string echo; //v1
      //echo += "[epoll server echo] ";
      //echo += request;
      std::string response = func_(request);//v2
      send(fd,response.c_str(),response.size(),0);
    }
  }

  void Sender()
  {

  }


  ~EpollServer()
  {
    listensock_.Close();
    epoller.Close();
  }

  private:
  uint16_t port_;
  Sock listensock_;
  Epoller epoller;

  struct epoll_event revs_[g_num]; //ready events 就绪事件数组

  func_t func_; //业务逻辑

  /*上层管理fd与自己缓冲区的数据结构, hashmap高效 */
  std::unordered_map<int, Connection*> connections_;

};
