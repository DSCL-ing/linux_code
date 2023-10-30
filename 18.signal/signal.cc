
#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>

//sighandler_t == void(*)(int)
void handler(int signu)
{
  std::cout<<"get a signal: " <<signu <<"pid: "<<getpid()<<std::endl;
  sleep(1);
}

void showBlock(sigset_t* oset)
{
  int signo =1;
  for(;signo<=31;++signo)
  {
    if(sigismember(oset,signo)==1) std::cout<<1;
    else std::cout<<0;
  }
  std::cout<<"\n";
}

int main()
{


  sigset_t set,oset;
  sigemptyset(&set);
  sigemptyset(&oset);
  sigaddset(&set,2);
  sigprocmask(SIG_SETMASK,&set,&oset);

  int cnt = 0;
  while(true)
  {
    showBlock(&oset) ;
    sleep(1);
    if(cnt == 10)
    {
      std::cout<<"recover block\n";
      sigprocmask(SIG_SETMASK,&oset,&set);
      showBlock(&set);
    }
    cnt++;

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
