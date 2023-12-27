#pragma once

#include<iostream>
#include <pthread.h>

// 让加锁和解锁实现自动化

// 封装锁
class Mutex 
{
public:
    Mutex(pthread_mutex_t *pmutex) : _pmutex(pmutex)
    {
        // 加载基本资源
    }
    void lock()
    {
        pthread_mutex_lock(_pmutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(_pmutex);
    }
    ~Mutex()
    {
        // 释放资源
    }

private:
    pthread_mutex_t *_pmutex; //使用外部的锁
};

// 守卫锁 --- RAII风格的锁
class LockGuard
{
public:
    LockGuard(pthread_mutex_t *pmutex) : _mutex(pmutex) //调用Mutex的构造函数
    {
        _mutex.lock();//调用Mutex提供的lock
    }
    ~LockGuard()
    {
        _mutex.unlock(); //调用Mutex提供的unlock
    }

private:
    Mutex _mutex; // 组合  //使用外部的锁
};
