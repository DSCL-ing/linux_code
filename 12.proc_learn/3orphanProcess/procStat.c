#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdbool.h>
int main()
{
  
    pid_t ret = fork(); 
    if(ret == 0)
    {
      while(true)
      {
          printf("我是子进程...ppid:%d,pid:%d\n",getppid(),getpid());
          sleep(1);
      }
    }
    else if(ret>0)
    {
      int cnt =10;
      while(true)
      {
          printf("我是父进程...ppid:%d,pid:%d\n",getppid(),getpid());
          sleep(1);
          if(cnt--<=0) break;
      }
    }

  return 0;
}
