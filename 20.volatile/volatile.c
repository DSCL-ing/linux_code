
#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<cassert>
#include<cstring>


static void showPending(sigset_t& pending)
{
  std::cout<<"当前进程的pending位图:";
  int signo =1;
  for(;signo<=31;++signo) {
    if(sigismember(&pending,signo)==1) std::cout<<1;
    else std::cout<<0;
  }
  std::cout<<"\n";
}

volatile int quit = 0; 

static void handler(int signu)
{
  printf("change quit from 0 to 1\n");
  quit = 1;
  printf("quit %d\n",quit);
}



int main()
{
  signal(2,handler);

  while(!quit);//不加括号,高频访问,骗编译器优化

  printf("man quit 正常");

  return 0 ;
}
