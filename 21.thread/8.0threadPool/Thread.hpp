#pragma once
#include <iostream>
#include <pthread.h>
#include <cstdio>
#include <string>
// 线程封装  encapsulation封装

// 一个类就是一个线程


//封装 对需要使用到类成员和routine时 的进一步封装
class Thread
{
public:
    enum ThreadStatus : int //枚举显示指定成员的类型
    {
        NEW = 0,
        RUNNING,
        EXIT
    };
    typedef void *(*func_t)(void *); // 函数指针,接收routine需要

public:
        //构造函数: 加载基本资源:初始化让线程跑起来前的基本数据
    Thread(int num, func_t func, void *args) // num 是我想给线程起的编号
        : _tid(0), _status(NEW), _func(func), _args(args)
    {
        // 对参数进行加工后再赋值给成员,完成初始化.
        char name[64];
        snprintf(name,sizeof(name), "thread-%d", num);
        _name = name;
    }
    ~Thread()
    {
        // 释放资源:释放线程
    }
    const std::string threadname() { return _name; } // 返回线程名
    int status() { return _status; } //返回线程执行的结果/状态
    pthread_t threadid() //返回线程id
    {
        if (_status != RUNNING)
        {
            return _tid;
        }
        else
            return 0;
    }

    //如果我想给线程传成员,需要另外定义一个routine,用于接收this,
    //不能直接使用外界的routine,已经定死了,必须要再次封装
    //静态函数没有隐藏成员this,符合runtine函数的参数列表要求,所以使用静态成员函数
    static void* runHelper(void* args) 
    {
        //但是静态函数不能直接访问成员--> 传this给args
        Thread *td = static_cast<Thread*>(args);
        //...修改_args...
        (*td)(); 
        return nullptr;
    }
    void operator()() //可以通过函数函数实现 --- 函数函数能扩展更多操作
    {
        //其他操作...
        
        if(_func!=nullptr)
        {
           _func(_args);
           //其他操作...
        }
        else return;
    }

    void run() // 让线程执行任务
    {
        int n = pthread_create(&_tid, nullptr, runHelper, (void*)this); //如果我想给线程传成员
        if (n != 0)
        {
            std::cerr << "pthread_create error\n";
            exit(1);
        }
        _status = RUNNING;
    }
    void join() // 等待线程
    {
        int n = pthread_join(_tid, nullptr);
        if (n != 0)
        {
            std::cerr << "pthread_join error\n";
            exit(2);
        }
        _status = EXIT;
    }

private:
    pthread_t _tid;       // 线程id
    std::string _name;    // 线程名字 = thread+用户标识ID
    ThreadStatus _status; // 线程状态 NEW,Running,exit
    func_t _func;         // 线程要执行的任务Routine
    void *_args;          // Routine需要的参数
};


//只执行routine的简单版
/*
class Thread
{
public:
    enum ThreadStatus : int
    {
        NEW = 0,
        RUNNING,
        EXIT
    };
    typedef void *(*func_t)(void *); // 函数指针,接收routine需要

public:
    Thread(int num, func_t func, void *args) //num 是我想给线程起的编号
        : _tid(0), _status(NEW), _func(func), _args(args)
    {
        // 初始化让线程跑起来前的基本数据
        char name[64];
        snprintf(name, 64, "thread-%d", num);
        _name = name;
    }
    ~Thread()
    {
        // 释放线程的基本数据
    }
    const std::string &threadname() { return _name; }
    int threadstatus() { return _status; }
    pthread_t threadid()
    {
        if (_status != RUNNING)
        {
            return _tid;
        }
        else
            return 0;
    }

    void run() // 让线程执行任务
    {
        int n = pthread_create(&_tid, nullptr, _func, _args);
        if(n!=0) exit(1);
        _status = RUNNING;
    }
    void join() // 等待线程
    {
        int n  = pthread_join(_tid, nullptr);
        if(n!=0) exit(2);
        _status = EXIT;
    }

public:
private:
    pthread_t _tid;       // 线程id
    std::string _name;    // 线程名字 = thread+用户标识ID
    ThreadStatus _status; // 线程状态 NEW,Running,exit
    func_t _func;         // 线程要执行的任务Routine
    void *_args;          // Routine需要的参数
};
*/
