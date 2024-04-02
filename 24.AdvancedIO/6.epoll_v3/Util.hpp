#include<iostream>
#include<unistd.h>
#include<fcntl.h>

class Util
{
public:
  static bool SetNonBlock(int fd)
  {
    int n = fcntl(fd,F_GETFL);
    if(n<0) return false;
    fcntl(fd,F_SETFL,n|O_NONBLOCK); // 或:加上没有的东西; 与:取出有的东西
    return true;
  }

};
