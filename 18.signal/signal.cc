
#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<cassert>

//sighandler_t == void(*)(int)
static void handler(int signu)
{
  std::cout<<"get a signal: " <<signu <<"pid: "<<getpid()<<std::endl;
  sleep(1);
}

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

int main()
{


  sigset_t set,oset;
  sigemptyset(&set);
  sigemptyset(&oset);

  for(int signo = 1 ; signo<=31;++signo)
  {
    sigaddset(&set,signo);
  }
  sigprocmask(SIG_SETMASK,&set,&oset);

  
  signal(2,handler);

  int cnt=0;
  while(true)
  {
    sigset_t pending;
    int n = sigemptyset(&pending);
    assert(n == 0);//一定会成功,就用assert
    (void)n; 
    sigpending(&pending);//获取未决表
    showPending(pending);//打印未决表
    if(cnt++ == 10)
    {
      std::cout<<"解除对2号信号的屏蔽\n";
      sigprocmask(SIG_SETMASK,&oset,nullptr);
    }
    
    sleep(1);
   
    


  }



  //signal(2,handler);
  //signal(3,handler);
  
 // signal(2,SIG_DFL);
  
  
  //signal(2,SIG_IGN);
  //sleep(1);
  //std::cout<<"DFL"<<std::endl;
  //std::cout<<"IGN"<<std::endl;
  //raise(2);
  
  //while(true)
  //{
  //  sleep(1);
  //}
  

  // signal(8,handler);
  // std::cout<<"除零错误"<<std::endl;
  // std::cout<<"除零错误"<<std::endl;
  // std::cout<<"除零错误"<<std::endl;
  // int a =1/0; //除零错误

    //std::cout<<"野指针"<<std::endl;
    //std::cout<<"野指针"<<std::endl;
    //std::cout<<"野指针"<<std::endl;
    //std::cout<<"野指针"<<std::endl;
    //int *p = NULL; 
    //*p = 10; //野指针


 // while(true)
 // {
 //   std::cout<<"我是一个进程,我正在运行 ... ,pid: "<<getpid()<<std::endl;
 //   sleep(1);
 // }


  return 0 ;
}
