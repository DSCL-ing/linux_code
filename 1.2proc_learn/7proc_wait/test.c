#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

#define TASK_NUM 10

void sync_disk()
{
  printf("同步磁盘任务...\n");
}
void sync_log()
{
  printf("同步日志任务...\n");
}   
void net_send()
{
  printf("网络发送任务...\n");
}

typedef void (*fun_t)();
fun_t other_task[TASK_NUM] = {NULL};
//默认是表结构, 如果不是表结构则不能使用一下遍历方式

int AddTask(fun_t fun)
{
  int i =0; 
  while(i<TASK_NUM)
  {
    if(other_task[i] == NULL)
      break; 
    ++i;
  }
   
    if(i==TASK_NUM)
      return -1;
    else
      other_task[i] = fun;
  return 0;
}

void InitTask()
{
  for(int i = 0 ;i<TASK_NUM;++i) other_task[i] = NULL;
  AddTask(sync_disk);
  AddTask(sync_log);
  AddTask(net_send);
}

void RunTest()
{
  for(int i = 0;i<TASK_NUM; ++i)
  {
    if(other_task[i]==NULL) continue;
    other_task[i]();
  }
}

int main()
{

  pid_t id = fork();
  InitTask(); 
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
        RunTest();
        sleep(1);
        continue;
      } else 
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
