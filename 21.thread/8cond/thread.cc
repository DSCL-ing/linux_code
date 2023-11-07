

#include<iostream>
#include<pthread.h>
#include<unistd.h>  
#include<cstring>
#include<cerrno>

const int num = 5;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void* active(void*args)
{
  const std::string name = static_cast<const char*>(args);

  while(true)
  {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond,&mutex);//直接进入休眠
    std::cout<<name<<" 活动"<<std::endl;
    pthread_mutex_unlock(&mutex);
  }

  //return nullptr;
}

int main()
{
  pthread_t tids[num];
  for(int i = 0 ; i<num;i++)
  {
    char *name = new char[32];
    snprintf(name,32,"thread-%d",i+1);
    pthread_create(tids+i,nullptr,active,(void*)name);
  }
  sleep(1);
  while(true)
  {
    std::cout<<"main wakeup thread ..." <<std::endl;
    // pthread_cond_signal(&cond); //逐一唤醒
    pthread_cond_broadcast(&cond); //全部唤醒
    sleep(1);
  }
  
  for(auto& tid: tids)
  {
    pthread_join(tid,nullptr);
  }

  return 0;
}

