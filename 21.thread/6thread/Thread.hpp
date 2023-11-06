#pragma once
#include <iostream>
#include <pthread.h>
// 线程封装

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
    Thread(int num, func_t func, void *args)
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
        pthread_create(&_tid, nullptr, _func, _args);
        _status = RUNNING;
    }
    void join() // 等待线程
    {
        pthread_join(_tid, nullptr);
        _status = EXIT;
    }
    void cancel() // 让线程结束
    {
        pthread_cancel(_tid);
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
