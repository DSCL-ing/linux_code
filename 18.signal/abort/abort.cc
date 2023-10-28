
#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>

void handler(int signu)
{
  std::cout<<"get a signal: " <<signu <<std::endl;
}

int main()
{

  signal(6,handler);
  //signal(3,handler);
  while(true)
  {
    std::cout<<"begin:"<<std::endl;
    sleep(1);
    abort();
    std::cout<<"end:"<<std::endl;


  }
  return 0 ;
}
