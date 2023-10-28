#include<iostream>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<string>
#include<stdlib.h>
#include<cstring>


//procedure 程序
void Usage(std::string proc )
{
  std::cout << "\tUsage: \n\t";
  std::cout << proc << "目标信号 目标进程"<<std::endl;
}

int main(int argc,char* argv[])
{
  if(argc!=3)
  {
    Usage(argv[0]);
    exit(1);
  }
  
  int signo = atoi(argv[1]); //no == No.
  int target_id = atoi(argv[2]);
  int n = kill(target_id,signo);
  if(n!=0)
  {
    std::cerr<<"errno:"<<errno<<"--->"<<strerror(errno)<<std::endl;
    exit(1);
  }



  return 0;
}
