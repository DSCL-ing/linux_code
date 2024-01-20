#ifndef __RING_QUEUE_HPP__
#define __RING_QUEUE_HPP__

#include<iostream>
#include<semaphore.h>
#include<vector>
#include<pthread.h>
#include<unistd.h>
#include<mutex>

static const int g_val = 50; //将全局变量限定于文件内,不冲突其他文件

template<typename T>
class RingQueue
{
  public:
    RingQueue(int num = g_val):_ring(num),_cap(num)
  {
    //不依赖于初始化参数的,或需要组合的,在括号里初始化
    _c_step = _p_step = 0; //生产和消费都是从队头开始

    pthread_mutex_init(&_c_mtx,nullptr); 
    pthread_mutex_init(&_p_mtx,nullptr); 

    sem_init(&_data_sem,0,0);
    sem_init(&_space_sem,0,num);
  }
    ~RingQueue()
    {
      pthread_mutex_destroy(&_c_mtx);
      pthread_mutex_destroy(&_p_mtx);
      sem_destroy(&_data_sem);
      sem_destroy(&_space_sem);
    }

    //将锁和信号量封装起来,抽象地更符合OS设计
    void P(sem_t &s) //为什么是引用? 根据函数体,需要修改信号量的内容,所以可以是指针或引用
    {
      sem_wait(&s); 
    }
    void V(sem_t &s)
    {
      sem_post(&s);
    }
    void lock(pthread_mutex_t &m)
    {
      pthread_mutex_lock(&m);
    }
    void unlock(pthread_mutex_t &m)
    {
      pthread_mutex_unlock(&m);
    }

    void push(const T& in)
    {
      P(_space_sem);
      lock(_p_mtx);
      _ring[_p_step++] = in; //插入数据到环形队列中
      _p_step %= _cap;
      unlock(_p_mtx);
      V(_data_sem);
    }

    void pop(T* out)
    {
      P(_data_sem); //申请数据信号量
      lock(_c_mtx);//加消费者锁
      *out = _ring[_c_step++]; //取数据
      _c_step %= _cap;
      unlock(_c_mtx); //解消费者锁
      V(_space_sem); //释放空间信号量
    }


  private:
    std::vector<T> _ring; //环形队列
    int _cap; // 环形队列的容量,为什么需要显式定义环形队列的容量?1.环形队列下标映射需要

    int _c_step;   //记录消费者当前到达的位置/消费者目前所在环形队列的下标/
    int _p_step;   //记录生产者当前到达的位置/生产者目前所在环形队列的下标/

    pthread_mutex_t _p_mtx; //维护生产者的竞争关系,一次只能有一个线程生产
    pthread_mutex_t _c_mtx; //维护消费者的竞争关系,一次只能有一个线程消费

    //维护生产者与消费者的同步关系
    sem_t _data_sem;  //_full;  
    sem_t _space_sem; //_empty;
};


#endif

