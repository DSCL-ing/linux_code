
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cstdio>

using namespace std;

int tickets = 1000;
pthread_mutex_t mutex;

void *threadRoutine(void *name)
{
  string tname = static_cast<const char *>(name);

  while (true)
  {
      pthread_mutex_lock(&mutex);
    if (tickets > 0)
    {
      usleep(2000);
      cout << tname << " get a ticket: " << tickets-- << endl;
      // printf("%s get a ticket: %d\n",name,tickets--);
      pthread_mutex_unlock(&mutex);
    }
    else
    {
      pthread_mutex_unlock(&mutex);
      break;
    }
    //usleep(1000);//现象原因:某线程执行太快,加锁解锁特别频繁,其他线程阻塞状态不能立即获得资源 ---> 导致只有一个线程工作
                //解决:让线程慢一点点..
  }
  return nullptr;
}

int main()
{
  pthread_mutex_init(&mutex,nullptr);
  pthread_t t[4];
  int n = sizeof(t) / sizeof(t[0]);

  for (int i = 0; i < n; i++)
  {
    char *tname = new char[64]; // C语言接口,传C字符串
    snprintf(tname, 64, "thread-%d", i + 1);
    pthread_create(t + i, nullptr, threadRoutine, tname);
  }

  for (int i = 0; i < n; i++)
  {
    pthread_join(t[i], nullptr);
  }
  pthread_mutex_destroy(&mutex);

  return 0;
}
