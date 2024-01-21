#pragma once

#include<iostream>
#include<thread>
#include<unordered_map>
#include<cstring>
#include<functional>

#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

#include<signal.h>


#include"ringQueue.hpp"
#include"err.hpp"

static const uint16_t default_port = 8888;
static const int backlog = 32; //待学

namespace a_very_long_name_namespace_server
{
  namespace v4 
  {

  }
  
  namespace v3 
  {

  }

  inline namespace v2
  {

    using func_t= std::function<std::string(const std::string&)>;
    class TCPServer
    {
      public:
        TCPServer(func_t fun,uint16_t port = default_port):port_(port),quit_(true),func_(fun)
      {


      }
        ~TCPServer()
        {

        }
        void initServer() 
        {
          //1.创建监听套接字
          listensock_ = socket(PF_INET,SOCK_STREAM,0);
          if(listensock_<0)
          {
            std::cerr<<"create socket error"<<strerror(errno)<<std::endl;
            exit(SOCKET_ERR);
          }

          //2.将套接字与套接字地址绑定
          struct sockaddr_in local;
          memset(&local,0,sizeof(local));
          local.sin_family = PF_INET;
          local.sin_addr.s_addr = INADDR_ANY;
          local.sin_port = htons(port_);
          int n = bind(listensock_,(sockaddr*)&local,sizeof(local));
          if( n < 0 )
          {
            std::cout<<"socket bind error :"<<strerror(errno)<<std::endl;
            exit(BIND_ERR);
          }

          //3.监听
          if(listen(listensock_,backlog)< 0) 
          {
            std::cout<<"socket listen error :"<<strerror(errno)<<std::endl;
            exit(LISTEN_ERR);
          }
          std::cout<<"TCP监听已启动"<<std::endl;

        }
        void start()
        {
          signal(SIGCHLD,SIG_IGN); //进程等待最好的解决方案,方案1 (还有一个方案2)
          quit_ = false;
          while(!quit_)
          {
            //4.获取连接
            struct sockaddr_in client;//accept成功后提取出客户端信息
            socklen_t len = sizeof(client);

            int sock = accept(listensock_,(sockaddr*)&client,&len);
            if(sock<0)
            {
              std::cerr<<"socket accept error : " <<strerror(errno)<<std::endl;
              continue;
            }
            //这个sock是用于提供服务的sock,listsock是用来建立连接的


            //5.获取新连接成功,开始服务
            std::string clientip = inet_ntoa(client.sin_addr);
            uint16_t clientport = client.sin_port;
            std::cout<<"获取新连接成功: "<<sock<<" form "<<listensock_<<", "\
              <<clientip<<"-"<<clientport<<std::endl;
            
            pid_t id = fork();
            if(id<0)
            {
              //可能是服务器没有能力处理更多的客户端了,关掉
              close(sock);
              continue; 
            }
            if(id == 0) 
            {
              //子进程拷贝父进程的文件描述符表
              //建议关掉不需要的fd
              close(listensock_) ;
              if(fork()>0) exit(0);//方案2:一行代码,不是最好,但有设计意义.原理:父进程只等待子进程.不会等待孙子进程.
              // 子进程中创建孙子进程,由孙子进程执行任务.关闭子进程后,孙子进程就成了孤儿进程,由OS领养,结束后由OS回收,无需等待
              // 缺点:开销太大
              service(sock,clientip,clientport);
              exit(0);
            }
            //父进程:一定要关闭掉不需要的fd
            close(sock);//sock交给子进程去处理了,父进程不需要就要关闭,不然会造成"文件描述符泄露"

            //进程等待
            //waitpid不可行,无论是轮询还是阻塞,只要是循环的都不行.因为总会卡住,
            //阻塞:会串行化,无法连接下一个客户端
            //轮询:需要先执行accept,然后才能wait,万一没有新的客户端,就会一直卡在accept.因此这样也不行

          }
        }
       
        void service(int sock,const std::string& clientip,uint64_t port)
        {
          //1.接收数据 
          //TCP面向字节流(文件字节流),使用read来读取 .UDP面向数据包需要使用recv接收
          //2.处理数据
          //3.回复处理结果

          std::string who = clientip+"-"+std::to_string(port);
          char buffer[1024];
          while(true)
          {
            ssize_t s = read(sock,buffer,sizeof(buffer)-1);
            if(s>0)
            {
              buffer[s] = '\0';
              std::cout<<who<< " : " << buffer<<std::endl;


              //处理数据
              std::string res = func_(buffer);
              write(sock,res.c_str(),res.size());
              //然后循环继续,继续对该IP进行服务.
            }
            else if(s == 0)//zero indicates end of file,零表示文件结束,在网络中表示对方把连接关闭了
            {
              //读到0个字节就是对方将连接关闭了/关闭sock的写端了,写端关闭,读端也要关闭
              close(sock);//关闭文件描述符,准备结束服务
              std::cout<<who<<" quit , me too."<<std::endl;
              break;//结束循环,结束服务
            }
            else
            {
              close(sock);//关闭文件描述符,准备结束服务
              std::cerr<<"read error: "<<strerror(errno)<<std::endl;
              break;//结束循环,结束服务
            }

          }

        }

      private:
        int listensock_;
        uint16_t port_;

        bool quit_;//服务器状态,false为运行,true为退出态. //暂时不知道好用在哪,先用着
        
        func_t func_;

        RingQueue<std::string> rq_;
        std::thread *p; 
        std::thread *c;

        std::unordered_map<std::string,sockaddr_in> onlineuser_;


    };
  }
  namespace v1
  {
    using func_t= std::function<std::string(const std::string&)>;
    class TCPServer
    {
      public:
        TCPServer(func_t fun,uint16_t port = default_port):port_(port),quit_(true),func_(fun)
      {


      }
        ~TCPServer()
        {

        }
        void initServer() 
        {
          //1.创建监听套接字
          listensock_ = socket(PF_INET,SOCK_STREAM,0);
          if(listensock_<0)
          {
            std::cerr<<"create socket error"<<strerror(errno)<<std::endl;
            exit(SOCKET_ERR);
          }

          //2.将套接字与套接字地址绑定
          struct sockaddr_in local;
          memset(&local,0,sizeof(local));
          local.sin_family = PF_INET;
          local.sin_addr.s_addr = INADDR_ANY;
          local.sin_port = htons(port_);
          int n = bind(listensock_,(sockaddr*)&local,sizeof(local));
          if( n < 0 )
          {
            std::cout<<"socket bind error :"<<strerror(errno)<<std::endl;
            exit(BIND_ERR);
          }

          //3.监听
          if(listen(listensock_,backlog)< 0) 
          {
            std::cout<<"socket listen error :"<<strerror(errno)<<std::endl;
            exit(LISTEN_ERR);
          }
          std::cout<<"TCP监听已启动"<<std::endl;

        }
        void start()
        {
          quit_ = false;
          while(!quit_)
          {
            //4.获取连接
            struct sockaddr_in client;//accept成功后提取出客户端信息
            socklen_t len = sizeof(client);

            int sock = accept(listensock_,(sockaddr*)&client,&len);
            if(sock<0)
            {
              std::cerr<<"socket accept error : " <<strerror(errno)<<std::endl;
              continue;
            }
            //这个sock是用于提供服务的sock,listsock是用来建立连接的


            //5.获取新连接成功,开始服务
            std::string clientip = inet_ntoa(client.sin_addr);
            uint16_t clientport = client.sin_port;
            std::cout<<"获取新连接成功: "<<sock<<" form "<<listensock_<<", "\
              <<clientip<<"-"<<clientport<<std::endl;
            
            service(sock,clientip,clientport);

          }
        }
       
        void service(int sock,const std::string& clientip,uint64_t port)
        {
          //1.接收数据 
          //TCP面向字节流(文件字节流),使用read来读取 .UDP面向数据包需要使用recv接收
          //2.处理数据
          //3.回复处理结果

          std::string who = clientip+"-"+std::to_string(port);
          char buffer[1024];
          while(true)
          {
            ssize_t s = read(sock,buffer,sizeof(buffer)-1);
            if(s>0)
            {
              buffer[s] = '\0';
              std::cout<<who<< " : " << buffer<<std::endl;


              //处理数据
              std::string res = func_(buffer);
              write(sock,res.c_str(),res.size());
              //然后循环继续,继续对该IP进行服务.
            }
            else if(s == 0)//zero indicates end of file,零表示文件结束,在网络中表示对方把连接关闭了
            {
              //读到0个字节就是对方将连接关闭了/关闭sock的写端了,写端关闭,读端也要关闭
              close(sock);//关闭文件描述符,准备结束服务
              std::cout<<who<<" quit , me too."<<std::endl;
              break;//结束循环,结束服务
            }
            else
            {
              close(sock);//关闭文件描述符,准备结束服务
              std::cerr<<"read error: "<<strerror(errno)<<std::endl;
              break;//结束循环,结束服务
            }

          }

        }

      private:
        int listensock_;
        uint16_t port_;

        bool quit_;//服务器状态,false为运行,true为退出态. //暂时不知道好用在哪,先用着
        
        func_t func_;

        RingQueue<std::string> rq_;
        std::thread *p; 
        std::thread *c;

        std::unordered_map<std::string,sockaddr_in> onlineuser_;


    };
  }



}

namespace ns_server = a_very_long_name_namespace_server; //us==namespace
