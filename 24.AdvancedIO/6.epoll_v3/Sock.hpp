#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"log.hpp"
#include"err.hpp"

static const int g_backlog = 32; //等待监听的连接队列长度
static const int defaultfd = -1;

class Sock
{
  public:
    Sock():sock_(defaultfd)
    {

    }
    ~Sock()
    {

    }

    //创建套接字
    void Socket()
    {
      sock_ = socket(PF_INET,SOCK_STREAM,0);
      if(sock_ < 0)
      {
        //套接字都不对,继续没意义 -> 致命错误
        logMessage(FATAL,"socket error , code: %d, errstring: %s ",
            errno,strerror(errno));
        exit(SOCKET_ERR);
      }

      //设置地址是可复用的
      int opt = 1;
      setsockopt(sock_,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt));

    }

    //绑定
    void Bind(uint16_t port) 
    {
      struct sockaddr_in local; 
      memset(&local,0,sizeof(local));
      local.sin_family = AF_INET;
      local.sin_addr.s_addr = INADDR_ANY;
      local.sin_port = htons(port);
      
      if(bind(sock_,(struct sockaddr*)&local,sizeof(local)) < 0)
      {
        logMessage(FATAL,"bind error , code: %d, errstring: %s ",
            errno,strerror(errno));
        exit(BIND_ERR);
      }
      
    }

    //启动监听
    void Listen()
    {
      if(listen(sock_,g_backlog) < 0)
      {
        logMessage(FATAL,"listen error, code: %d, errstring: %s",
            errno,strerror(errno));
        exit(LISTEN_ERR);

      }

    }

    //获取连接套接字和客户端的ip与端口号
    int Accept(std::string *clientip,uint16_t *clientport, int *err)
    {
      struct sockaddr_in temp;
      memset(&temp,0,sizeof(temp));
      socklen_t len ;
      int sock = accept(sock_,(sockaddr*)&temp,&len);
      *err = errno;  // 需要根据错误码做出正确的读写
      if(sock < 0)
      {
        //获取连接错误的话,可能说明资源紧张,但服务器还能维护之前的连接.
        logMessage(WARNING," accept error, code: %d, errorstring: %s",
            errno,strerror(errno));
      }
      else
      {
        *clientip = inet_ntoa(temp.sin_addr);
        *clientport = ntohs(temp.sin_port);
      }

      return sock;
    }

    //连接服务器,返回套接字/句柄
    int Connect(const std::string& serverip,const uint16_t& port)
    {
      struct sockaddr_in server;
      memset(&server,0,sizeof(server));
      server.sin_addr.s_addr = inet_addr(serverip.c_str());
      server.sin_port = htons(port);
      return connect(sock_,(sockaddr*)&server,sizeof(server));
      //客户端也可打日志.略
    }

    //返回套接字
    int Fd()
    {
      return sock_;
    }
    
    //结束通信,关闭套接字
    void Close()
    {
      if(sock_!=defaultfd)
        close(sock_);
    }

  private:
    int sock_; //通信端点;在服务端仅用于监听,通信交由accept构建的套接字

};
