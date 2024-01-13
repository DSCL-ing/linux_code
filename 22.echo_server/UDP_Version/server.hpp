#pragma once

#include<iostream>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<cerrno>
#include<cstring>
#include<cstdlib>

static const uint16_t default_port = 8080;

class UdpServer
{
public:
  UdpServer(std::string ip ,uint16_t port = default_port):port_(port),ip_(ip)
  {

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
      exit(errno);
    }
    std::cout<<"create socket success: " <<sock_<<std::endl;

    //2.给服务器指明IP和端口号Port 
    
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    //服务器和客户端是需要相互交互的.所以必须要让双方都知道自己的位置, 所以需要把端口号和IP地址放到网络报文中,因此需要调整网络字节序
    //服务器必须要有固定的端口号和IP,方便用户定位和访问
    local.sin_port = htons(port_);//主机转网络,16位短整数
     
    local.sin_addr.s_addr = htonl(ip_); //

  }

  void Start()
  {

  }

private:
  int sock_;  //udp服务器自己的套接字
  uint16_t port_; //udp服务器自己的端口号

  //一般输入的IP都是点分十进制的.所以是字符串
  std::string ip_; //服务器自己的IP
}; // end  UdpServer
