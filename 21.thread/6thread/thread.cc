
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cstdio>
#include"Thread.hpp"

using namespace std;

int tickets = 1000;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 



void *routine(void* args)
{

  // while (true)
  // {
  //   pthread_mutex_lock(&tdata->_mutex);
  //   if (tickets > 0)
  //   {
  //     usleep(2000);
  //     cout << tdata->_tname << " get a ticket: " << tickets-- << endl;
  //     // printf("%s get a ticket: %d\n",name,tickets--);
  //     pthread_mutex_unlock(&tdata->_mutex);
  //   }
  //   else
  //   {
  //     pthread_mutex_unlock(&tdata->_mutex);
  //     break;
  //   }
  //   //usleep(1000);//现象原因:某线程执行太快,加锁解锁特别频繁,其他线程阻塞状态不能立即获得资源 ---> 导致只有一个线程工作
  //               //解决:让线程慢一点点..
  // }
  // return nullptr;
}



int main()
{
  Thread t1(1,routine,(void*)"hello1");
  cout << "thread name: " << t1.threadname() << " thread id: " << t1.threadid() << ",thread status: " << t1.threadstatus() << std::endl;


  return 0;
}
