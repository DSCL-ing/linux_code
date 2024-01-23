#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <iostream>
#include <string>
#include <functional>

using cb_t = std::function<void(int fd,const std::string& ip,const uint16_t &port)>;

class Task
{
public:
    Task() //默认的构造函数有什么用? 消费者接收资源时,能够空初始化定义对象,拷贝.
    {}

    Task(int fd,std::string ip,uint16_t port,cb_t cb)
      :_sock(fd),_ip(ip),_port(port),_cb(cb)
    {}

    ~Task()
    {
    }

    void operator()() // object(); === func();
    {
      _cb(_sock,_ip,_port);
    }


private:
    int _sock;
    std::string _ip;
    uint16_t _port;
    cb_t _cb;
};

#endif
