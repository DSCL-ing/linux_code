#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include"comm.hpp"
#include<cstring>
#include<cerrno>
int main()
{
  umask(0000);
  //1创建命名管道
  int n = mkfifo(fifoname.c_str(),mode) ;
  if(n<0)
  {
    // 错误格式:  errno:xx-->错误信息
    std::cout<<"errno:"<<errno<<"-->"<<strerror(errno)<<std::endl;
    return 1;
  }
  //2.打开管道,以读方式
  int rfd = open(fifoname.c_str(),O_RDONLY);
  if(rfd<0)
  {
    std::cout<<"errno:"<<errno<<"-->"<<strerror(errno)<<std::endl;
    return 2;
  }
  //3.开始通信
  char buffer[NUM] = {0};
  while(true)
  {
    buffer[0]=0;//C字符串只要第一个个是0就是空串了
    ssize_t n = read(rfd,buffer,sizeof(buffer)-1);
    if(n>0)
    {
      buffer[n] = 0;
    }
    else if(n==0)
    {
      std::cout<<" 客户端停止发送数据,终止当前进程" <<std::endl;
      break;
    }
    else
    {
			std::cout<<"errno:"<<errno<<"-->"<<strerror(errno)<<std::endl;
			return 3;
		}
  }
  //3.关闭管道
  close(rfd);
  return 0;
}
