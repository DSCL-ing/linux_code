
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

int main()
{

  //signal(2,handler);
  //signal(3,handler);



  // signal(8,handler);
  // std::cout<<"除零错误"<<std::endl;
  // std::cout<<"除零错误"<<std::endl;
  // std::cout<<"除零错误"<<std::endl;
  // int a =1/0; //除零错误

    std::cout<<"野指针"<<std::endl;
    std::cout<<"野指针"<<std::endl;
    std::cout<<"野指针"<<std::endl;
    std::cout<<"野指针"<<std::endl;
  int *p = NULL; 
  *p = 10; //野指针


 // while(true)
 // {
 //   std::cout<<"我是一个进程,我正在运行 ... ,pid: "<<getpid()<<std::endl;
 //   sleep(1);
 // }


  return 0 ;
}
