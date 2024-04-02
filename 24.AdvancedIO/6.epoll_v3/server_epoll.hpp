#pragma once
#include <iostream>
#include "Sock.hpp"
#include"Epoller.hpp"
#include<cassert>
#include<functional>
#include<unordered_map>
#include"Util.hpp"
#include"log.hpp"
#include"err.hpp"

/*
   一.服务器基本结构
   初始化+启动  //test
   */

const static uint16_t defaultport = 8888;
const static int bsize = 1024;  //buffer size :字节流缓冲区的大小
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

    //用户信息 , only debug
    std::string clientip_;
    uint16_t clientport_;
  
    //还可以给conn带上每个fd要关心的事件,用于根据不同的事件做不同的策略
    uint32_t events_;
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
    //AddConnection(listensock_.Fd(),EPOLLIN);//LT
    AddConnection(listensock_.Fd(),EPOLLIN|EPOLLET); //ET

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
    //判断是否要设置成ET模式,是则把需要关心的所有文件描述符设置非阻塞
    if(events&EPOLLET) Util::SetNonBlock(fd);
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

    //保存关心的事件
    conn->events_ = events;
    bool r = epoller.AddEvent(fd,events); //r == ret
    assert(r);
    (void)r;

    logMessage(INFO,"AddConnection succes, fd: %d,\
       clientinfo:[%s,%d]",fd,clientip.c_str(),clientport);
  }

  void Accepter(Connection* conn)
  {
    //先do一次,如果是ET模式,就继续循环,否则只执行一次(即LT)
    do
    {

      (void)conn;
      std::string clientip; 
      uint16_t clientport;
      int err;
      int sock = listensock_.Accept(&clientip,&clientport,&err);
      //logMessage(INFO,"get a new link...");
      if(sock > 0)
      {
        logMessage(INFO,"%s:%d已经连上服务器",clientip.c_str(),clientport);
        //AddConnection(sock,EPOLLIN,clientip,clientport); //LT
        AddConnection(sock,EPOLLIN|EPOLLET,clientip,clientport);   //ET
        continue;//进行下次读取,加不加都可以,用于代码逻辑提醒
      }
      else
      {
        if(err == EAGAIN || err == EWOULDBLOCK)
        {
          //底层没数据了,说明已取完 -- 唯一出口
          break;
        }
        else if(err == EINTR)
        {
          //因信号中断读取,重新读
          continue;
        }
        else
        {
          //遇到真错误,没办法,但还是继续读.后续代码还可以加入错误处理
          logMessage(WARNING,"accept error, code: %d, errstring: %s",err,strerror(err));
          continue;
        }

      }

    }while(conn->events_&EPOLLET);
  }

  void Recver(Connection*conn)
  {
    do
    {
      char buffer[bsize];
      ssize_t n = recv(conn->fd_,buffer,sizeof(buffer),0);
      if(n>0)
      {
        buffer[n] = 0;//需要加上C语言分隔符,不然inbuffer不知道有多长
        conn->inbuffer_+=buffer; //+=是会自动去掉\0,不用担心
        
        //可以边读取边分析... 1
        logMessage(DEBUG,"fd: %d, inbuffer: %s",conn->fd_,conn->inbuffer_.c_str());
      }
      else if(n == 0)
      {
        conn->excepter_(conn);
      }
      else
      {
        if(n == EAGAIN)
        {
          //说明底层没数据了,可以结束循环  -- 唯一出口
          break;
        }
        else if(n == EINTR) continue; //信号中断
        else 
        {
          conn->excepter_(conn);// 真的出错 
          continue;
        }
      }

    }while(conn->events_& EPOLLET);
    //读完了,也可以根据基本协议进行数据分析 2
  }

  void Sender(Connection*conn)
  {

  }

  void Excepter(Connection*conn)
  {

  }

  bool ConnIsExists(int fd)
  {
    return connections_.find(fd) != connections_.end();
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
