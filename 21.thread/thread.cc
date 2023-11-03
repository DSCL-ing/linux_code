
#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<cstring>

#define NUM 10

void* thread_run(void* args)//void可以接受任意类型
{
  char* name = (char*) args;
  while(1){
    printf("new  thread running,my thread name is: %s\n",name);
    break;
  }
  delete[] name;
  sleep(1);
  return nullptr;
}

int main()
{
  pthread_t tids[NUM];
  for(int i = 0; i<NUM;++i){
    char* tname = new char[64];
    //char tname[64];
    snprintf(tname, 64,"thread-%d",i+1);
    pthread_create(tids+i,nullptr,thread_run,tname);
  }

  for(int i = 0 ; i<NUM ; ++i){
   int n =  pthread_join(tids[i],nullptr);
   if(n!=0) std::cout<<"phread_join error"<<std::endl;
  }

  std::cout<<"all thread quit"<<std::endl;

  //while(1){
  //  printf("main thread running\n");
  //  sleep(1);
  //}

  return 0;
}
