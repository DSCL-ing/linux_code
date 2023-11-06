
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cstdio>
#include "Thread.hpp"
#include "lockGuard.hpp"

using namespace std;

int tickets = 1000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *args)
{
  const std::string message = static_cast<const char *>(args);
  while (true)
  {
    {
      LockGuard lockguard(&mutex); //临时对象  -- RAII风格的锁
      if (tickets > 0)
      {
        usleep(2000);
        cout << "message: " << message << " ,get a ticket: " << tickets-- << endl; // 临界区
        // tickets--;
      }
      else
      {
        break;
      }
    } //出了花括号自动释放 -- 临时对象的生命周期在最近的花括号
  }
  return nullptr;
}

int main()
{
  Thread t1(1, routine, (void *)"hello1");
  Thread t2(1, routine, (void *)"hello2");
  Thread t3(1, routine, (void *)"hello3");
  Thread t4(1, routine, (void *)"hello4");
  // cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.threadstatus() << std::endl;
  // cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.threadstatus() << std::endl;
  t1.run();
  t2.run();
  t3.run();
  t4.run();

  //必须要写join,或者分离线程.否则主线程一挂,所有线程也跟着挂
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  return 0;
}
