#pragma once

#include<iostream>
#include<thread>
#include<unordered_map>
#include<cstring>

#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>


#include"ringQueue.hpp"
#include"err.hpp"

static const uint16_t default_port = 8888;
static const int backlog = 32; //待学

namespace a_very_long_name_namespace_server
{
  inline namespace v1
  {
    class TCPServer
    {
      public:
        TCPServer(uint16_t port = default_port):port_(port),quit_(true)
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

        }
        void start()
        {
          quit_ = false;
          while(!quit_)
          {
            //4.获取连接
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int sock = accept(listensock_,(sockaddr*)&client,&len);
            if(sock<0)
            {

            }

            
          }

          


        }
        void Rcev()
        {

        }
        void Send()
        {

        }

      private:
        int listensock_;
        uint16_t port_;

        bool quit_;//服务器状态,false为运行,true为退出态. //暂时不知道好用在哪,先用着

        RingQueue<std::string> rq_;
        std::thread *p; 
        std::thread *c;

        std::unordered_map<std::string,sockaddr_in> onlineuser_;


    };
  }
  namespace v2 
  {

  }

  namespace v3 
  {

  }

  namespace v4 
  {

  }

}

namespace ns_server = a_very_long_name_namespace_server; //us==namespace
