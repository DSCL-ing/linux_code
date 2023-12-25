

#ifndef __RING_QUEUE_HPP__
#define __RING_QUEUE_HPP__

#include<iostream>
#include<semaphore.h>
#include<vector>
#include<pthread.h>
#include<unistd.h>
#include<mutex>

const int g_val = 5;

template<typename T>
class RingQueue
{
public:
  RingQueue()
  {
   pthread_mutex_init(&_mtx,nullptr); 
   sem_init(_empty,0,g_val);
   sem_init(_full,0,0);

  }

  //生产活动
  void producer()
  {
    sem_wait(&_empty);
    pthread_mutex_lock(&_mtx); 
    //sleep(1000);
    //_v.push_back()
    sem_post(&_full);
    pthread_mutex_unlock(&_mtx);
  } 

  //消费活动 
  void consumer()
  {
    sem_wait(&_full);
    std::lock_guard<std::mutex> m(mtx);
    pthread_mutex_lock(&_mtx);
    sem_pos(&_empty);
    pthread_mutex_unlock(&_mtx);

  }

private:
  std::vector<T> _v;
  std::mutex mtx;
  pthread_mutex_t _mtx;
  //pthread_cond_t _full; 
  //pthread_cond_t _empty;
  sem_t _full;  
  sem_t _empty;
};


#endif





