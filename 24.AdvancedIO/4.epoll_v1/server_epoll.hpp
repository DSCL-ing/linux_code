#pragma once
#include <iostream>
#include "Sock.hpp"
#include"Epoller.hpp"
#include<cassert>
#include<functional>

/*
一.服务器基本结构
  初始化+启动  //test
*/

const static uint16_t defaultport = 8888;

/*业务逻辑*/
using func_t = std::function<std::string(std::string)>;

class EpollServer
{
const static int g_num = 128;
public:
    EpollServer(func_t func,uint16_t port = defaultport)
      :func_(func),port_(port)
    {
    }
    void InitServer()
    {
      listensock_.Socket();
      listensock_.Bind(port_);
      listensock_.Listen();
      epoller.Create();
      std::cout<<"epoll占用的fd: "<<epoller.Fd()<<std::endl;

      logMessage(INFO,"init server success");
    }
    void Start()
    {
      bool r = epoller.AddEvent(listensock_.Fd(),EPOLLIN); //r == ret
      assert(r);
      (void)r;
      while(true)
      {
        //sleep(1);
        //如果直接accept,则服务器将以阻塞方式运行
        //要非阻塞,需要先多路转接

        int timeout = -1;
        int n = epoller.Wait(revs_,g_num,timeout);
        switch(n) 
        {
          case 0:
            logMessage(INFO,"timeout...");
            break;
          case -1:
            logMessage(WARNING,"epoll_wait failed");//failed:失败
            break;
          default:
            logMessage(INFO,"有%d个事件就绪",n); //一般测试都是1,因为计算机处理很快,很大压力才有多个
            HandlerEvents(n);//事件处理例程
        }

      }

    }

    void HandlerEvents(int num)
    {
      for(int i = 0; i<num ;i++)
      {
        sleep(1);

        int fd = revs_[i].data.fd;
        uint32_t events = revs_[i].events;
        
        logMessage(INFO,"当前正在处理%d上的%s",fd,EPOLLIN&events?"EPOLLIN":"OTHER");

        //目前无法保证读到一个完整的报文
        //完整报文应由应用层协议规定.
        
        //读事件就绪
        if(events & EPOLLIN) 
        {
          //EPOLLIN分为新连接就绪和数据可以读取事件

          if(fd == listensock_.Fd()) //取走新连接
          {
            std::string clientip; 
            uint16_t clientport;
            int sock = listensock_.Accept(&clientip,&clientport);
            //logMessage(INFO,"get a new link...");
            if(sock < 0)
            {
              continue; //失败就不要了,去取下一个
            }
            logMessage(INFO,"%s:%d已经连上服务器",clientip.c_str(),clientport);
            bool r = epoller.AddEvent(sock,EPOLLIN);
            assert(r);
            (void)r;

          }
          else //取数据
          {
            char request[1024];
            ssize_t s = recv(fd,request,sizeof(request)-1,0); //给缓冲区最后留个空位,防止放不下\0
            if(s<0)
            {
              logMessage(WARNING,"recv error,client quit");
              epoller.DelEvent(fd);
              close(fd);
            }
            else if(s == 0)
            {
              //返回值为0一般表示读到末尾
              logMessage(INFO,"client quit...");
            }
            else
            {
              request[s] = 0; //给末尾加上\0
              /*telnet的行分隔符是\r\n,仅仅n-1打印会覆盖*/
              /*验证:*/
                //buffer[s-1] = 0;
                //buffer[s-2] = 0;
              //std::cout<<"client# "<<request<<std::endl;

              /*echo server*/
              //std::string echo;
              //echo += "[epoll server echo] ";
              //echo += request;
              std::string response = func_(request);
              send(fd,response.c_str(),response.size(),0);
            }
            

          }

        }
        else
        {

          
        }
      }

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

};
