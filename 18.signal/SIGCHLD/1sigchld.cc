
#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>



void handler(int signo)
{
  std::cout<<"get a signal: " <<signo <<std::endl;
  std::cout<<"who: "<<getpid()<<std::endl;
  exit(0);
  
}

int main()
{

  signal(SIGCHLD,handler);
  
  int id = fork();
  if(id==0) {
    int cnt = 5; 
    while(cnt--){
      std::cout<<"我是子进程,我的pid是:"<<getpid()<<" ppid:"<<getppid()<<"\n";
      sleep(1);
    }
    exit(1);
  }


  while(1)
  {
    sleep(1);
  }
  return 0 ;
}
