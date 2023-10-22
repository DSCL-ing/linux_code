#include<iostream>

#include"comm.hpp"

int main()
{
  //1.服务端提供了管道,客户端只需要打开管道
  int wfd = open(fifoname.c_str(),O_WRONLY);
  if(wfd<0)
  {
		std::cout<<"errno:"<<errno<<"-->"<<strerror(errno)<<std::endl;
    return 1;
  }


  return 0;
}
