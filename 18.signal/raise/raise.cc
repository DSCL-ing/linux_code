
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

  signal(2,handler);
  //signal(3,handler);
  while(true)
  {
    sleep(1);
    raise(2);
  }
  return 0 ;
}
