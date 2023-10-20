#include<iostream>
#include<string>
#include<unistd.h>
#include<cassert>
#include<vector>
#include"Task.hpp"
#include<cstdlib>
using namespace std;

const int gnum = 5;//创建管道的数量  -- 代替宏的全局常量


Task task;

struct endPoint 
{
  int _child_id;
  int _write_fd;
  endPoint(int id,int fd)
    : _child_id(id)
    , _write_fd(fd)
  {}
};

void WaitCommand()
{
  int command;
  while(true)
  {
    cout<<2;
    int n = read(0,&command,sizeof(int));
    if(n == sizeof(int)) task.Execute(command);
    else if(n == 0) break;//没命令了
    else break; //异常
  }
}

void createProcess(std::vector<endPoint> *end_points)
{  for(int i = 0; i<gnum; ++i)
  {
    //1.创建管道
    int pipefd[2] = {0};
    int n = pipe(pipefd);
    assert(n == 0); (void)n;

    //2.创建子进程
    pid_t id = fork();
    assert(id != -1);


    if(id>0)//子进程
    {
      close(pipefd[1]);
      
      //希望子进程从标准输入中读取
      dup2(pipefd[1],0);

      //然后子进程开始等待,从标准输入中获取命令
      WaitCommand(); //循环执行任务,直到进程终止

      //结束收尾
      close(pipefd[0]);
      exit(0);
    }
    
    //父进程
    close(pipefd[0]);
    end_points->push_back(endPoint(id,pipefd[0]));
  }

}


int main()
{
//构建一个多管道进程控制程序
  std::vector<endPoint> end_points; cout<<1;
  //构建基本结构,父进程通过管道控制子进程,父进程写,子进程读
  createProcess(&end_points);

//父进程管理子进程
  while(true){
  //1.选择任务
  int command = COMMAND_LOG; 
  //2.选择进程
  int index = rand()%3; 
  //3.下发任务
  write(end_points[index]._write_fd,&command,sizeof(command));
  cout<<"1";
  sleep(1);
}


  return 0;
}
