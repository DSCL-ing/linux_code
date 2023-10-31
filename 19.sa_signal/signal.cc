
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

static void handler(int signu)
{
  std::cout<<"get a signal: " <<signu <<"pid: "<<getpid()<<std::endl;
  int cnt = 10;
  while(cnt)
  {
    cnt--;

    sigset_t pending;
    sigemptyset(&pending);
    sigpending(&pending);
    showPending(pending);
    sleep(1);
  }
}



int main()
{

  struct sigaction act,oldact;
  memset(&act,0,sizeof(act));
  memset(&oldact,0,sizeof(oldact));
  act.sa_handler = handler;
  act.sa_flags = 0; 
  //以上等价于signa
  
  sigemptyset(&act.sa_mask);
  
  //处理sigaction中的signo号的同时阻塞3,4,5号信号 
  sigaddset(&act.sa_mask,3); 
  sigaddset(&act.sa_mask,4); 
  sigaddset(&act.sa_mask,5); 

  sigaction(2,&act,&oldact);

  while(true) sleep(1);

  return 0 ;
}
