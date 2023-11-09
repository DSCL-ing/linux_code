#pragma once

#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

// typedef int Data_t;
typedef class Task Data_t;

extern const int g_cap = 5;

template <typename T>
class BlockQueue
{
public:
  BlockQueue(int cap = g_cap) : _cap(cap)
  {
    pthread_cond_init(&_producer_cond, nullptr);
    pthread_cond_init(&_consumer_cond, nullptr);
    pthread_mutex_init(&_mutex, nullptr);
  }

  bool isFull()
  {
    //队列数量 等于 任务容量时就是满的
    return _q.size() == _cap;
  }

  bool isEmpty()
  {
    return _q.empty();
  }

  void push(const T &in) // 生产
  {
    // 使用临界资源,先上锁
    pthread_mutex_lock(&_mutex);
    //只能在临界区内部,判断临界资源是否就绪 
    // if(isfull())
    while (isFull()) // 为了防止多线程时误唤醒后还是满的,需要改为while连续条件判断
    {
      //当仓库满了,不能继续生产,放不下,要去阻塞等待唤醒,不要浪费资源一直请求

      pthread_cond_wait(&_producer_cond, &_mutex); //线程要有能力自己阻塞. 锁只有一把,不能带着锁去等待,必须要释放锁后才去休眠,所以参数也带了锁.如果被唤醒后,还没解锁,则阻塞在wait中,重新申请锁直到申请成功才能离开
      
      //唤醒后,如果仓库还是满,则循环继续等待.直到满足条件才能离开
    }
    //生产
    _q.push(in);
    //先解锁和先唤醒无所谓,先唤醒没解锁也要阻塞,先解锁没唤醒只能等唤醒,直到解锁和唤醒
    
    //唤醒策略...
    //if(_q.size>=_cap/2) pthread_cond_signal(&_consumer_cond);
    pthread_cond_signal(&_consumer_cond); //要有唤醒对方的能力
    pthread_mutex_unlock(&_mutex); //解锁
  }

  void pop(T *out) // 消费 -- 同push,生产
  {
    pthread_mutex_lock(&_mutex);
    while (isEmpty()) 
    {
      //当超市空,不能再拿,要去阻塞等待唤醒,不要浪费资源一直请求
      pthread_cond_wait(&_consumer_cond, &_mutex);
    }
    //消费
    *out = _q.front();
    _q.pop();
    
    //唤醒策略...
    pthread_mutex_unlock(&_mutex);
    pthread_cond_signal(&_producer_cond);
  }

  ~BlockQueue()
  {
    //不会自动销毁吗?不会,因为是动态分配的锁
    pthread_mutex_destroy(&_mutex);  //输出型参数说明,_mutex仅仅是一个变量,里面动态分配的才是主要的
    pthread_cond_destroy(&_producer_cond); 
    pthread_cond_destroy(&_consumer_cond);
  }

private:
  std::queue<Data_t> _q;
  int _cap; // capacity -- 任务需要的容量

  pthread_mutex_t _mutex; // 提供锁用于保护队列 -- 一份公共资源一个锁

  // 维护同步关系,保持合理不浪费,并且各自使用各的队列
  pthread_cond_t _producer_cond;
  pthread_cond_t _consumer_cond;
};
