
#pragma once

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"err.hpp"
#include"log.hpp"

#include<signal.h>


void Daemon()
{
  //都是一些固定的写法



  //1.忽略掉常见的信号(忽略的行为会被子进程继承)
  signal(SIGPIPE,SIG_IGN); //关闭文件描述符/管道时,会收到SIGPIPE信号
  signal(SIGCHLD,SIG_IGN); //子进程结束时,会受到SIGCHLD信号
  //...  遇到就可以添加上.为什么要忽略,有空查一下

    //2.让自己成为组长    (护进程在启动时,当前进程不能是组长)
  if(fork()>0) exit(0);//创建子进程,让父进程退出,然后我就不是组长了,组长虽然挂掉,但组还在

  
  //3.新建会话,自己成为会话的首进程
  pid_t ret  = setsid(); 
  if((int)ret == -1)
  {
    logMessage(FATAL,"daemon error, code:%d, string:%s ",errno,strerror(errno));
    //[FATAL] [2024-1-23 20:41:29] [6144]  daemon error, code:1, string:Operation not permitted 
    exit(SETSID_ERR);
  }


  //4.更改工作目录,看情况修改
   //chdir("/");

  //5.处理0,1,2
  //守护进程后,不应该再往显示器上输出
  int fd = open("/dev/null",O_RDWR);
  if(fd<0)
  {
    logMessage(FATAL,"open error, code:%d, string:%s ",errno,strerror(errno));
    exit(OPEN_ERR);
  }
  dup2(fd,0);
  dup2(fd,1);
  dup2(fd,2);

}

