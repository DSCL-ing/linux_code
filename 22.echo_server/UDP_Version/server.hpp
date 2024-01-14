#pragma once

#include<iostream>
#include<string>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<cerrno>
#include<cstring>
#include<cstdlib>


#include"err.hpp"
static const uint16_t default_port = 8080;

class UdpServer
{
public:
  //UdpServer(std::string ip ,uint16_t port = default_port):port_(port),ip_(ip)
  UdpServer(uint16_t port = default_port):port_(port)
  {
      std::cout<<" , server port: " << port <<std::endl;
  }
  ~UdpServer()
  {

  }

  void InitServer()
  {
    //1. 打开socket接口,打开网络文件
    sock_ = socket(AF_INET,SOCK_DGRAM,0);
    if(sock_<0)
    {
      std::cout<<"create socket error :"<<strerror(errno)<<std::endl;
      exit(SOCKET_ERR);
    }
    std::cout<<"create socket success: " <<sock_<<std::endl;

    //2.现在本地给本地套接字字段指明IP和端口号Port 
    
    struct sockaddr_in local;
    memset(&local,0,sizeof(local));
    local.sin_family = AF_INET;
    //服务器和客户端是需要相互交互的.所以必须要让双方都知道自己的位置, 所以需要把端口号和IP地址放到网络报文中,因此需要调整网络字节序
    //服务器必须要有固定的端口号和IP,方便用户定位和访问
    local.sin_port = htons(port_);//主机转网络,16位短整数
    //local.sin_addr.s_addr = inet_addr(ip_.c_str());//需要把点分十进制转成uint32的IP地址类型
    local.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY值为全零.转不转网络字节序无所谓.
    
    //3.将准备好的本地套接字字段和网络文件套接字字段绑定
    int n = bind(sock_,(struct sockaddr*)&local,sizeof(local)); //sockaddr_in和sockaddr和sockaddr_un等大小是不一样的,所以把类型大小作参数传进来
    if(n!= 0 )
    {
      std::cout<<"bind socker error :"<<strerror(errno)<<std::endl;
      exit(BIND_ERR);
    }
    std::cout<<"bind socket success ! " << std::endl;
  }

   void Start()
  {
    while(true)
    {
      char buffer[1024];
      struct sockaddr_in peer;
      socklen_t len = sizeof(peer);
      ssize_t n = recvfrom(sock_,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&peer,&len);
      if(n>0)
      {
        buffer[n] = 0;
      }
      else
      {
        
      }
    }
  }

private:
  int sock_;  //udp服务器自己的套接字
  uint16_t port_; //udp服务器自己的端口号

  //一般输入的IP都是点分十进制的.所以是字符串
  //std::string ip_; //服务器自己的IP --------------------- 云服务器版本不需要手动提供IP地址
}; // end  UdpServer
