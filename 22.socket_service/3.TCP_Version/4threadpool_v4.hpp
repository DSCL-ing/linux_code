#ifndef THREADPOOL_V4_HPP 
#define THREADPOOL_V4_HPP

#include<iostream>
#include<vector>
#include<unistd.h>
#include<pthread.h>
#include<queue>
#include"Thread.hpp" //版本2:引入封装的线程
#include"lockGuard.hpp" //版本3:引入守护锁
//版本4:引入单例

//懒汉单例:
/*
1.构造私有化 
2.封拷贝
3.提供不依赖实例的静态对象指针,和获取单例的静态方法
4.提供GC或Destroy,帮助销毁 //未实现 -- 没封析构,用智能指针析构
*/

static const int N = 5;



















template<class T> //任务的类型
class ThreadPool
{
private: 
  ThreadPool(int num = N):_num(num)
  {
    pthread_mutex_init(&_mtx,nullptr);
    pthread_cond_init(&_cond,nullptr);
  }
  ThreadPool(const ThreadPool& tp) = delete;
  ThreadPool& operator=(const ThreadPool& tp)= delete;
public:
  static ThreadPool<T>* getInstance()
  {
    if(nullptr==_instance)    
    {
      LockGuard LockGuard(&_instance_mtx);
      if(nullptr == _instance)
      {
        _instance = new ThreadPool<T>(); //在类域内,能够访问私有的构造函数
        _instance->init();
        _instance->start();
        //可以在这里init和start,也可以在main
      }
    }
    return _instance;
  }
  ~ThreadPool()
  {
    for(auto&t:_threads)
    {
      t.join();       
    }
    pthread_mutex_destroy(&_mtx);
    pthread_cond_destroy(&_cond);
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
  //void lock()
  //{
  //  pthread_mutex_lock(&_mtx);
  //}
  //void unlock()
  //{
  //  pthread_mutex_unlock(&_mtx);
  //}

  
  void pushTask(const T& t)
  {
    LockGuard LockGuard(&_mtx);
    _tasks.push(t);
    threadWakeup();
  }

  T popTask() //返回拷贝:pop后的值没有了,不保存
  {
    T t = _tasks.front();
    _tasks.pop();
    return t;
  }
  pthread_mutex_t* get_mutex()
  {
    return &_mtx;
  }

  static void* threadRoutine(void*args)
  {
     pthread_detach(pthread_self()); 
     ThreadPool<T>* tp = static_cast<ThreadPool<T>*>(args);
     while(true) 
     {
       T t;
       {
         LockGuard(tp->get_mutex()); //bug? 为什么能直接调用非静态成员函数
         while(tp->isEmpty())
         {
           tp->threadWait();
         }
         t = tp->popTask();
       }
      t();
     }
  }

  void init() //线程准备工作
  {
    for(int i = 0; i<_num; i++)
    {
      _threads.push_back(Thread(i,threadRoutine,this));
    }
    
  }
  void start() //线程启动
  {
    for(auto&t:_threads)  
    {
      t.run();
    }
  }
  
  void check()
  {
    for (auto &t : _threads)
    {
        std::cout << t.threadname() << " running..." << std::endl;
    }
  }

private:

  std::vector<Thread> _threads; //管理线程的数据结构
  int _num; //记录线程数
  pthread_cond_t _cond;    //没有任务时的线程休眠队列

  std::queue<T> _tasks;     //任务队列,利用stl自动扩容的特性
  pthread_mutex_t _mtx; //保护任务队列(共享资源)的锁

//还可以给_instance加volatile关键字 防止过度优化,但后续可能有很多细节要处理.可加可不加
  static ThreadPool* _instance; //不依赖实例的单例指针
  static pthread_mutex_t _instance_mtx;
};

template<class T>
 ThreadPool<T>* ThreadPool<T>::_instance = nullptr;

template<class T>
 pthread_mutex_t ThreadPool<T>::_instance_mtx = PTHREAD_MUTEX_INITIALIZER;


#endif
