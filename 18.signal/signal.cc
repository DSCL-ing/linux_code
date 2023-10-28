
#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>

//sighandler_t == void(*)(int)
void handler(int signu)
{
  std::cout<<"get a signal: " <<signu <<std::endl;
}

int main()
{

  signal(2,handler);

  while(true)
  {
    std::cout<<"我是一个进程,我正在运行 ... ,pid: "<<getpid()<<std::endl;
    sleep(1);
  }


  return 0 ;
}
