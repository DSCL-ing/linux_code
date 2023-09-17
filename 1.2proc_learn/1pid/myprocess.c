#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
    int x = 100;
   pid_t ret = fork();
   if(ret == 0)
   {
     while(1)
     {
       printf("我是一个子进程; pid:%d; ppid:%d; x:%d; &x:%p; \n",getpid(),getppid(),x,&x);
       sleep(1);
     }
   }
   else if(ret>0)
   {
      while(1)
      {
       printf("我是一个父进程; pid:%d; ppid:%d; x:%d; &x:%p; \n",getpid(),getppid(),x,&x);
       x = 4321;
       sleep(1);
      }
   }
   else
   {}

//  2  
//      printf("AAAAAAAAAAAAAAAAAAAAAAA; pid:%d; ppid:%d; \n",getpid(),getppid());
//      pid_t ret = fork();
//      printf("BBBBBBBBBBBBBBBBBBBBBBB; pid:%d; ppid:%d; ret:%d; &ret:%p; \n",getpid(),getppid(),ret,&ret);
//      

//  1      
//    while(1)
//    {
//      printf("hello process!我的进程id是:%d,父进程是%d\n",getpid(),getppid());
//      sleep(1);
//    }
//
}
