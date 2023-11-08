#pragma once

#include<iostream>
#include<queue>
#include<pthread.h>

typedef int Data_t;

template<typename T>
class BlockQueue
{
  public:
    BlockQueue()
    {

    }
    void push(const T &in)
    {

    }
    void pop(T *out)
    {

    }
    ~BlockQueue()
    {

    }
   private:
    std::queue<Data_t> _q;
    int _cap;//capacity

    pthread_mutex_t mutex;//提供锁用于保护队列
    pthread_cond_t cond; //维护同步关系,保持合理不浪费


};

