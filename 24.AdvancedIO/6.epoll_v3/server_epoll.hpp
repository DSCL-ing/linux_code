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
class Connection;

/*业务逻辑*/
using func_t = std::function<std::string(std::string)>;
using callback_t = std::function<void (Connection*)>;


/* 每个fd都需要有自己的缓冲区 */
class Connection //管理服务器的动作,相当于关节
{
  public:
    Connection(int fd,std::string clientip,uint16_t clientport )
      :fd_(fd),clientip_(clientip),clientport_(clientport)
    {} 
    ~Connection();
    void Register(callback_t recver,callback_t sender,callback_t excepter)
    {
      recver_ = recver;
      sender_ = sender;
      excepter_ = excepter;
    }

    //IO信息
    int fd_;
    std::string inbuffer_;
    std::string outbuffer_;
    //IO处理函数
    callback_t recver_;
    callback_t sender_;
    callback_t excepter_;

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
  void Dispacher()//事件派发器
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
  void LoopOnce(int timeout)  
  {
    int n = epoller.Wait(revs_,g_num,timeout);
    for(int i = 0; i<n ;i++) //只有n大于0(有就绪事件)才会处理
    {
      sleep(1);

      int fd = revs_[i].data.fd;
      uint32_t events = revs_[i].events;

      logMessage(INFO,"当前正在处理%d上的%s",fd,EPOLLIN&events?"EPOLLIN":"OTHER");

      if(events&EPOLLIN) connections_[fd]->recver_(connections_[fd]);
      if(events&EPOLLOUT) connections_[fd]->sender_(connections_[fd]);
      if(events&EPOLLERR || events&EPOLLHUP) connections_[fd]->excepter_(connections_[fd]);


    }

  }
  
  /*将添加管理操作封装*/
  void AddConnection(int fd,uint32_t events,std::string clientip = "127.0.0.1" ,uint16_t clientport = defaultport)
  {
    Connection* conn = new Connection(fd,clientip,clientport); 
    connections_.insert(std::pair<int,Connection*>(fd,conn));

    //AddConnection在回调前必经.因此可以承担起注册工作
    if(fd == listensock_.Fd())
    {
      //listensock的recver_是Accept
        conn->Register(std::bind(&EpollServer::Accepter,this,std::placeholders::_1),nullptr,nullptr);
    }
    else
    {
        conn->Register(
            std::bind(&EpollServer::Recver,this,std::placeholders::_1), 
            std::bind(&EpollServer::Sender,this,std::placeholders::_1),
            std::bind(&EpollServer::Excepter,this,std::placeholders::_1)
            );
    }

    bool r = epoller.AddEvent(fd,events); //r == ret
    assert(r);
    (void)r;

    logMessage(INFO,"AddConnection succes, fd: %d,\
       clientinfo:[%s,%d]",fd,clientip.c_str(),clientport);
  }

  void Accepter(Connection* conn)
  {
    (void)conn;
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

  void Recver(Connection*conn)
  {

  }

  void Sender(Connection*conn)
  {

  }

  void Excepter(Connection*conn)
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
