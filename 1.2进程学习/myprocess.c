#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
      printf("AAAAAAAAAAAAAAAAAAAAAAA; pid:%d; ppid:%d; \n",getpid(),getppid());
      fork();
      printf("BBBBBBBBBBBBBBBBBBBBBBB; pid:%d; ppid:%d; \n",getpid(),getppid());
//    while(1)
//    {
//      printf("hello process!我的进程id是:%d,父进程是%d\n",getpid(),getppid());
//      sleep(1);
//    }
//
}
