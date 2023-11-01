
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t id = 0;

void waitProcess(int signo)
{
  printf("捕捉到一个信号: %d,who: %d\n", signo, getpid());
  sleep(5);
  while (1)
  {
    pid_t res = waitpid(-1, nullptr, WNOHANG); // 返回等到的子进程 的pid,非阻塞,保证进程正常执行
    if (res > 0)
    {
      printf("wait success, res: %d, id: %d\n", res, id); // res和id是否相同
    }
    else
      break; //循环至没有子进程zombie
  }
  printf("handler done.\n");
}


int main()
{

  // signal(SIGCHLD, waitProcess);
  signal(SIGCHLD,SIG_IGN);

  int i = 1;
  for (; i <= 10; ++i)
  {

    id = fork();
    if (id == 0)
    {
      int cnt = 5;
      while (cnt--)
      {
        std::cout << "我是子进程,我的pid是:" << getpid() << " ppid:" << getppid() << "\n";
        sleep(1);
      }
      exit(1);
    }
  }

  while (1)
  {
    sleep(1);
  }
  return 0;
}
