#pragma once
#include <iostream>
#include "Sock.hpp"

const static uint16_t defaultport = 8888;

class EpollServer
{
public:
    EpollServer(uint16_t port = defaultport):port_(port)
    {
    }
    void InitServer()
    {
      listensock_.Socket();
      listensock_.Bind(port_);
      listensock_.Listen();

    }
    void Start()
    {
      while(true)
      {
        sleep(1);
        //如果直接accept,则服务器将以阻塞方式运行
        //要非阻塞,需要先多路转接
        

      }

    }
    ~EpollServer()
    {

    }

private:
    uint16_t port_;
    Sock listensock_;
};
