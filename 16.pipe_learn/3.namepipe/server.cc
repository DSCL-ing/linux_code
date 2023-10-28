#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include"comm.hpp"
#include<cstring>
#include<cerrno>
int main()
{
  umask(0000);

  if (access(fifoname.c_str(), F_OK) == 0)
  {
    remove(fifoname.c_str());
    printf("文件 %s 存在,已删除\n", fifoname.c_str());
  }

  //1创建命名管道
  int n = mkfifo(fifoname.c_str(),mode) ;
  if(n!=0)
  {
    // 错误格式:  errno:xx-->错误信息
    std::cout<<"errno:"<<errno<<"-->"<<strerror(errno)<<std::endl;
    return 1;
  }
  std::cout<<"create fifo file success"<<std::endl;
  //2.打开管道,以读方式
  int rfd = open(fifoname.c_str(),O_RDONLY);
  if(rfd<0)
  {
    std::cout<<"errno:"<<errno<<"-->"<<strerror(errno)<<std::endl;
    return 2;
  }
  std::cout<<"open fifo success,begin ipc" <<std::endl;
  //3.开始通信
  char buffer[NUM] = {0};
  while(true)
  {
    buffer[0]=0;//C字符串只要第一个个是0就是空串了
    ssize_t n = read(rfd,buffer,sizeof(buffer)-1);
    if(n>0)
    {
      buffer[n] = 0;
      std::cout<<"client# "<< buffer <<std::endl; //换行自动刷新
    }
    else if(n==0)
    {
      std::cout<<" 客户端已停止,服务端也关闭" <<std::endl;
      break;
    }
    else
    {
			std::cout<<"errno:"<<errno<<"-->"<<strerror(errno)<<std::endl;
			return 3;
		}
  }
  //3.关闭管道
  close(rfd);

  //4.删除管道文件
  unlink(fifoname.c_str()); //客户端强制关闭后,服务端能正常结束,所以unlink放在服务端合适
  std::cout<<"removed fifo file" << std::endl;
  return 0;
}
