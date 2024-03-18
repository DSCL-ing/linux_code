#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>
#include<cstdio>

void SetNonBlock(int fd)
{
  int fl = fcntl(fd,F_GETFL);
  if(fl < 0 )
  {
    std::cout<<"error string :"<<strerror(errno)<<"error code:"<<errno<<std::endl;
    return ;
  }
  fcntl(fd,F_SETFL,fl|O_NONBLOCK);
}

int main()
{
  char buffer[64];
  SetNonBlock(0);
  while(true)
  {
    printf(">>>");
    fflush(stdout);
    ssize_t n = read(0,buffer,sizeof(buffer)-1);
    if(n>0)
    {
      buffer[n] = '\0';
      std::cout<<"echo# "<<buffer<<std::endl;
    }
    else if (n == 0)
    {
      std::cout<<"end file"<<std::endl;
      break;
    }
    else 
    {
      //std::cout<<"read error??"<<std::endl; //文件描述符有问题时出错
      std::cout<<"read error, error string :"<<strerror(errno)<<"error code:"<<errno<<std::endl;
      //break;
    }
    sleep(1);
  }
  
  return 0;
}
