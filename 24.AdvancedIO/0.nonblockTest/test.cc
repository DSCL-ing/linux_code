#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>
#include<cstdio>
#include<functional>
#include<vector>

//测试任务例程
void PrintLog()
{ std::cout<<"打印日志例程..."<<std::endl;}
void OperMysql()
{std::cout<<"操作数据库例程..."<<std::endl;}
void CheckNet()
{std::cout<<"检测网络状态例程..."<<std::endl;}

using func_t = std::function<void(void)>;
std::vector<func_t> funcs;

void LoadTask()
{
  funcs.push_back(PrintLog);funcs.push_back(OperMysql);funcs.push_back(CheckNet);
}

void HandlerAllTask()
{
  for(const auto &func:funcs) func();
}

void SetNonBlock(int fd)
{
  int fl = fcntl(fd,F_GETFL);
  if(fl < 0 )
  {
    std::cerr<<"error string :"<<strerror(errno)<<"error code:"<<errno<<std::endl;
    return ;
  }
  fcntl(fd,F_SETFL,fl|O_NONBLOCK);
}


int main()
{
  char buffer[64];
  SetNonBlock(0);
  LoadTask();
  while(true)
  {
      sleep(1);
    std::cout<<">>>"<<std::flush;
    //printf(">>>");
    //fflush(stdout);
    ssize_t n = read(0,buffer,sizeof(buffer)-1);
    if(n>0)
    {
      buffer[n] = '\0';
      std::cout<<"echo# "<<buffer<<std::endl;
    }
    else if (n == 0)
    {
      std::cout<<"end file"<<std::endl;
      break;
    }
    else 
    {
      //std::cout<<"read error??"<<std::endl; //文件描述符有问题时出错,有那些出错原因?
      if(errno == EAGAIN||errno == EWOULDBLOCK) 
      {
        //errno == 11,非真出错,只是因为底层没数据
        std::cout<<"data not ready!"<<std::endl;
        HandlerAllTask();
        continue;
      }
      else if(errno == EINTR)
      {
        continue; //因信号中断,也是重新获取
      }
      else
      {
        //这里才是真正出现错误的情况
        std::cerr<<"read error, error string :"<<strerror(errno)<<"error code:"<<errno<<std::endl;
        //break;
      }
    }
  }

  return 0;
}
