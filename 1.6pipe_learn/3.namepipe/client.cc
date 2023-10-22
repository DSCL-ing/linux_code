
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
  while (true)
  {
    buffer[0] = 0; // 置成c空串
    char* msg = fgets(buffer, sizeof(buffer), stdin); //成功返回字符串,错误返回null
    assert(msg); //测试必须不能为空
    (void)msg;
    buffer[strlen(buffer) - 1] = 0; //把换行干掉

    if(strcasecmp(buffer,"quit") == 0) break;
    else if(strcasecmp(buffer,"exit") == 0) break;

    // 2.2往服务端写数据
    ssize_t n = write(wfd, buffer, strlen(buffer)); // write返回值目前没有用,等网络
    assert(n>=0);
    (void)n;
  }

  return 0;
}
