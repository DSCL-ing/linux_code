
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cstdio>

using namespace std;

int tickets = 1000;

class TData
{
  public:
    TData(const std::string& name,pthread_mutex_t& mutex)
      :_tname(name),_mutex(mutex) 
    {}
    ~TData()
    {}
  public:
    std::string _tname; //线程名
    pthread_mutex_t& _mutex; //引用局部域中定义的锁
};


void *threadRoutine(void* args)
{
  TData* tdata = static_cast<TData *>(args);

  while (true)
  {
    pthread_mutex_lock(&tdata->_mutex);
    if (tickets > 0)
    {
      usleep(2000);
      cout << tdata->_tname << " get a ticket: " << tickets-- << endl;
      // printf("%s get a ticket: %d\n",name,tickets--);
      pthread_mutex_unlock(&tdata->_mutex);
    }
    else
    {
      pthread_mutex_unlock(&tdata->_mutex);
      break;
    }
    //usleep(1000);//现象原因:某线程执行太快,加锁解锁特别频繁,其他线程阻塞状态不能立即获得资源 ---> 导致只有一个线程工作
                //解决:让线程慢一点点..
  }
  return nullptr;
}



int main()
{
  pthread_mutex_t mutex;//局部锁,需要让所有线程看见的话,要传进routine
  pthread_mutex_init(&mutex,nullptr);

  pthread_t t[4];
 
  int n = sizeof(t) / sizeof(t[0]);
  for (int i = 0; i < n; i++)
  {
    char name[64];
    snprintf(name,64,"thread-%d",i+1);
    TData *tdata = new TData(name,mutex);
    pthread_create(t + i, nullptr, threadRoutine, tdata);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_join(t[i], nullptr);
  }
  pthread_mutex_destroy(&mutex);

  return 0;
}
