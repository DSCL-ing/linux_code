
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t id = fork();
  if (id == 0)
  {
    std::cout << "异常问题...." << std::endl;
    std::cout << "异常问题...." << std::endl;
    std::cout << "异常问题...." << std::endl;
    int *p = nullptr;
    *p = 1100;
    std::cout << "异常问题...." << std::endl;
    std::cout << "异常问题...." << std::endl;
    std::cout << "异常问题...." << std::endl;
    exit(0);
  }
  int status = 0;
  waitpid(id,&status,0);
  std::cout<<"exit code:   " <<((status>>8) & 0xFF) <<std::endl;
  std::cout<<"exit signal: " <<(status & 0x7F) <<std::endl;
  std::cout<<"core dump flag: "<<((status>>7)&0x1) <<std::endl;

  return 0;
}
