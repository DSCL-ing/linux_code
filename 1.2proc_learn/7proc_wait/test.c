#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{

  pid_t id = fork();
  
  if(id == 0) 
  {
    int cnt = 5;
    while(cnt)
    {
      printf("我是子进程,我有%ds, pid:%d, ppid:%d\n",cnt,getpid(),getppid());
      --cnt;
      sleep(1);
    }
    exit(111);
  }
  while(1)
  {   
      int status = 0;
      pid_t ret_id = waitpid(id,&status,WNOHANG);
      if(ret_id<0)
      {
        printf("waitpid error\n");
        exit(1);
      }
      else if(ret_id == 0)
      {
        printf("子进程还没退出,可以执行别的\n");
        sleep(1);
        continue;
      }
      else 
      {
        if(WIFEXITED(status)) //如果status中信号为0,即没有信号时执行
        {
          printf("我是父进程,等待子进程成功,ret_id:%d, pid:%d, ppid:%d,child exit code:%d\n" ,ret_id,getpid(),getppid(),WEXITSTATUS(status)); 
        }
        else
        {
            printf("我是父进程,等待子进程成功,ret_id:%d, pid:%d, ppid:%d,child exit signal:%d\n" ,ret_id,getpid(),getppid(),status&0x7F); 
        }
          break;
      }
  }
  // 2.waitpid(pid,status,0)的使用
//  pid_t id = fork();
//  if(id == 0) 
//  {
//    int cnt = 5;
//    while(cnt)
//    {
//      printf("我是子进程,我有%ds, pid:%d, ppid:%d\n",cnt,getpid(),getppid());
//      --cnt;
//      sleep(1);
//      int a = 0;
//      a = a/0 ;
//    }
//    if(1==1) exit(0);
//    exit(111);
//  }
//  int status = 0;
//  pid_t ret_id = waitpid(id,&status,0);
//   printf("我是父进程,等待子进程成功,ret_id:%d, pid:%d, ppid:%d,child exit code:%d , child exit signal:%d\n" ,ret_id,getpid(),getppid(),(status>>8)&0xFF,status&0x7F); 

   //1.wait:接受任何进程
//  sleep(10);
//  pid_t ret_id = wait(NULL);
//  printf("我是父进程,等待子进程成功,ret_id:%d, pid:%d, ppid:%d\n",ret_id,getpid(),getppid());
//  sleep(5);
  return 0;
}
