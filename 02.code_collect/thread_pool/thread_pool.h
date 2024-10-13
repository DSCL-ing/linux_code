#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__
#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include<functional>

//不能用于:
/*
  1.有序处理任务 --> 线程池任务处理是无序的
  2.任务有强逻辑关联 --> 可能导致线程池串行化 
 */

class ThreadPool  {
  public:
    //封拷贝,保证唯一入口-->单例
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    static ThreadPool& instance() {
      static ThreadPool ins;
      return ins;
    }

    //封装异步任务:void()类型
    using Task = std::packaged_task<void()>;
    
    //析构
    ~ThreadPool() {
      stop();
    }

    template <class F, class... Args>
      auto commit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using RetType = decltype(f(args...));

        //如果线程池处于停止状态,则直接返回空future,表示任务不再处理
        if (stop_.load())
          return std::future<RetType>{};

        //构造局部任务对象的智能指针,智能指针用途:在传入任务队列前后,析构当前局部对象时仍保留到任务队列中,引用计数++
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));  //绑定成一个无参的函数,传给ptask封装
                                                                          //bind将参数绑定,使传递/调用更方便,减少参数传递
                                                                          //完美转发:参数传给bind时属性不丢失(引用,左右值..)

        //取出future,返回给用户用于获取结果
        std::future<RetType> ret = task->get_future();
        

        //传入任务队列
        {
          std::lock_guard<std::mutex> cv_mt(cv_mt_);  //保证传入过程互斥
          tasks_.emplace([task] { (*task)(); });  //再包一层成pool::task类型对象,传给任务队列等待执行
                                                  //lambda引用task,智能指针计数++,保证局部task不被析构,成功转移对象
        }
        cv_lock_.notify_one();  //生产了一个任务,唤醒一个线程
       
        //返回给用户
        return ret;//用户可以直接get,或者wait,wait_for...等等
      }

    int idleThreadCount() {
      return thread_num_;
    }



  private:
    //构造函数: 默认5个线程
    ThreadPool(unsigned int num = 5)
      : stop_(false) {
        {
          if (num < 1)
            thread_num_ = 1;  //至少存在一个线程
          else
            thread_num_ = num;
        }
        start();  //启动线程池
      }
    
    //启动线程池
    void start() {
      //逐个构造线程
      for (int i = 0; i < thread_num_; ++i) {
        //emplace_back,减少一次构造:任务对象
        pool_.emplace_back([this]() {
            //每个线程的执行逻辑:
            while (!this->stop_.load()) {
            Task task;
            {
            std::unique_lock<std::mutex> cv_mt(cv_mt_); //普通条件变量需要unique_lock;
            this->cv_lock_.wait(cv_mt, [this] {
                return this->stop_.load() || !this->tasks_.empty(); //停止状态时,需要启动线程让其走完后退出,否则会阻塞导致无法退出;
                });
            //如果任务队列为空,则可以直接返回
            if (this->tasks_.empty())
            return;
            task = std::move(this->tasks_.front()); //取出任务对象(ptask只允许移动)
            this->tasks_.pop(); //移除掉(无效的)任务对象
            }

            //开始任务处理
            this->thread_num_--;  
            task();
            this->thread_num_++;
            }
            });
      }
    }
    void stop() {
      stop_.store(true);
      cv_lock_.notify_all();  //唤醒休眠/阻塞的线程,处理完剩余的任务再结束

      //join
      for (auto& td : pool_) {
        if (td.joinable()) {  //保证join的是没被分离的线程
          std::cout << "join thread " << td.get_id() << std::endl;
          td.join();
        }
      }
    }
  private:
    std::mutex               cv_mt_;      //互斥量
    std::condition_variable  cv_lock_;    //条件变量
    std::atomic_bool         stop_;       //标志:标志启停,原子
    std::atomic_int          thread_num_; //线程个数
    std::queue<Task>         tasks_;      //任务队列,类型都是task
    std::vector<std::thread> pool_;       //池:一个线程数组
};
#endif  // !__THREAD_POOL_H__
