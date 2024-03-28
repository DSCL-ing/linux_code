#pragma once
#include<iostream>
#include<sys/epoll.h>
#include<unistd.h>
#include"log.hpp"
#include"err.hpp"
#include<cstring>

//流+日志+业务

static const int defaultfd = -1;
static const int g_size = 128;

class Epoller //辅助构建epoll
{
  public:
    Epoller():epfd_(defaultfd)
    {

    }
    ~Epoller()
    {

    }
    void Create()
    {
      epfd_ = epoll_create(g_size);
      if(epfd_<0)
      {
        logMessage(FATAL,"epoll_create error, code: %d, errstring: %s",
            errno,strerror(errno));
        exit(EPOLL_CREATE_ERR);
      }
    }
    bool AddEvent(int fd,uint32_t events)
    {
      struct epoll_event ev;
      ev.events = events;
      ev.data.fd = fd;
      int n = epoll_ctl(epfd_,EPOLL_CTL_ADD,fd,&ev);
      if(n<0)
      {
        logMessage(FATAL,"epoll_ctl addEvent error, code: %d, errstring: %s",
            errno,strerror(errno));
        return false; //添加失败虽然致命,但不至于结束服务器,下次再添加
      }
      return true;
    }

    bool DelEvent(int fd) 
    {
      //epoll操作fd时要求fd必须合法
      return epoll_ctl(epfd_,EPOLL_CTL_DEL,fd,nullptr);
    }

    int Wait(struct epoll_event *revs,int num,int timeout)
    {
      return epoll_wait(epfd_,revs,num,timeout);
    }
    int Fd()
    {
      return epfd_;
    }
    void Close()
    {

      if(epfd_!=defaultfd)
      {
        close(epfd_);
      }
    }

  
  private:
    int epfd_; //指向epoll模型的文件描述符

};
