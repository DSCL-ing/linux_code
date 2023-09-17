#include<stdio.h>
#include<unistd.h>
int main()
{
  //
 pid_t ret = fork(); 
    if(ret == 0)
    {
      while(1)
      {
          printf("我是子进程...ppid:%d,pid:%d\n",getppid(),getpid());
          sleep(1);
      }
    }
    else if(ret>0)
    {
      while(1)
      {
          printf("我是父进程...ppid:%d,pid:%d\n",getppid(),getpid());
          sleep(1);
      }
    }

  //3.查看进程暂停态T
//    int i = 0;
//    while(1)
//    {
//      printf("我在运行中...%d\n",++i);
//      sleep(1);
//    }

// 2.进程运行态R,没有调用外设
//    while(1)
//    {
//        
//    }

 //  1.进程宏观休眠状态S,--SR切换态,频繁调用外设,printf屏幕
 // while(1)
 // {
 //  printf("我是一个进程!\n");
 // }
  return 0;
}
