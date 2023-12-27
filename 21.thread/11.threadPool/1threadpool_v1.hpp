#ifndef __THREADPOOL_V1_HPP__ 
#define __THREADPOOL_V1_HPP__

#include<iostream>
#include<vector>
#include<unistd.h>
#include<pthread.h>
#include<queue>

static const int N = 5;

template<class T> //任务的类型
class ThreadPool
{
public:
  ThreadPool(int num = N):_num(num),_threads(num)
  {
    pthread_mutex_init(&_mtx,nullptr);
    pthread_cond_init(&_cond,nullptr);
  }
  ~ThreadPool()
  {
    for(int i = 0 ; i<_threads.size();i++)
    {
      pthread_join(_threads[i],nullptr);
    }
    pthread_mutex_destroy(&_mtx);
    pthread_cond_destroy(&_cond);
  }

  void lock()
  {
    pthread_mutex_lock(&_mtx);
  }
  void unlock()
  {
    pthread_mutex_unlock(&_mtx);
  }


  void pushTask(const T& t)
  {
    lock();
    _tasks.push(t);
    unlock();
    threadWakeup();
  }

  T popTask() //返回拷贝:pop后的值没有了,不保存
  {
    T t = _tasks.front();
    _tasks.pop();
    return t;
  }

  bool isEmpty()
  {
    return _tasks.empty();
  }

  void threadWait()
  {
    pthread_cond_wait(&_cond,&_mtx);
  }

  void threadWakeup()
  {
    pthread_cond_signal(&_cond);
  }

  static void* threadRoutine(void*args)
  {
    //pthread_detach(pthread_self()); //把自己分离,不用主线程等待
    ThreadPool<T>* tp = static_cast<ThreadPool<T>*>(args);
     while(true) //线程循环运行
     {
      tp->lock();
      while(tp->isEmpty())
      {
        tp->threadWait();
      }
      T t = tp->popTask();
      tp->unlock();
      t();
      std::cout<<"处理任务,结果:"<<t.formatRes()<<std::endl;
     }
  }

  void init() //线程准备工作
  {
    
  }
  void start() //线程启动
  {
    for(int i = 0;i<_num;i++)  
    {
      pthread_create(&_threads[i],nullptr,threadRoutine,(void*)this);
    }
        
  }


private:
  std::vector<pthread_t> _threads; //管理线程的数据结构
  int _num; //记录线程数
  pthread_cond_t _cond;    //没有任务时的线程休眠队列

  std::queue<T> _tasks;     //任务队列,利用stl自动扩容的特性
  pthread_mutex_t _mtx; //保护任务队列(共享资源)的锁
        
};

#endif
