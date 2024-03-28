#pragma once
#include <iostream>
#include "Sock.hpp"
#include"Epoller.hpp"
#include<cassert>

/*
一.服务器基本结构
  初始化+启动  //test

*/

const static uint16_t defaultport = 8888;

class EpollServer
{
const static int g_num = 128;
public:
    EpollServer(uint16_t port = defaultport):port_(port)
    {
    }
    void InitServer()
    {
      listensock_.Socket();
      listensock_.Bind(port_);
      listensock_.Listen();
      epoller.Create();
    }
    void Start()
    {
      bool r = epoller.AddEvent(listensock_.Fd(),EPOLLIN); //r == ret
      assert(r);
      (void)r;
      while(true)
      {
        sleep(1);
        //如果直接accept,则服务器将以阻塞方式运行
        //要非阻塞,需要先多路转接

        int timeout = 1000;
        int n = epoller.Wait(revs,g_num,timeout);
        switch(n) 
        {
          case 0:
            logMessage(INFO,"timeout...");
            break;
          case -1:
            logMessage(WARNING,"epoll_wait failed");//failed:失败
            break;
          default:
            logMessage(INFO,"有%d个事件就绪了",n);
            HandlerEvents(n);//事件处理例程
        }

      }

    }

    void HandlerEvents(int num)
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
    
    struct epoll_event revs[g_num]; //ready events 就绪事件数组

};
