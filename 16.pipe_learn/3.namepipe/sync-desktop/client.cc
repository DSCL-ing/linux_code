
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "comm.hpp"
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <cassert>

int main()
{
  // 1.服务端提供了管道,客户端只需要打开管道
  int wfd = open(fifoname.c_str(), O_WRONLY);
  if (wfd < 0)
  {
    std::cout << "errno:" << errno << "-->" << strerror(errno) << std::endl;
    return 1;
  }

  // 2.正常通信
  // 2.1 从标准输入中获取数据
  char buffer[NUM];
  std::cout << "请输入你的消息# ";
  while (true)
  {
    system("stty raw");
    int c = getchar();       //服务端关闭,client也会关闭....
    system("stty -raw");

    ssize_t n = write(wfd, (char *)&c, sizeof(char));
    assert(n >= 0);
    (void)n;
  }

  close(wfd);



  return 0;
}
