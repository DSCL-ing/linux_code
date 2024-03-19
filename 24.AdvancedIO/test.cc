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
    std::cerr<<"error string :"<<strerror(errno)<<"error code:"<<errno<<std::endl;
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
      sleep(1);
      //std::cout<<"read error??"<<std::endl; //文件描述符有问题时出错,有什么问题? 看下面
      if(errno == EAGAIN||errno == EWOULDBLOCK) 
      {
        //errno == 11,非真出错,只是因为底层没数据
        std::cout<<"data not ready!"<<std::endl;
        continue;
      }
      else if(errno == EINTR)
      {
        continue; //因信号中断,也是重新获取
      }
      else
      {
        //这里才是真正出现错误的情况
        std::cerr<<"read error, error string :"<<strerror(errno)<<"error code:"<<errno<<std::endl;
        //break;
      }
    }
  }

  return 0;
}
