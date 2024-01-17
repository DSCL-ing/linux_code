#include"server.hpp"
#include<memory>
#include<string>
#include<ctype.h>
#include<string.h>
#include<vector>

void Usage(std::string proc)
{
  std::cout<< "Usage:\n\t" << proc <<" port\n"<<std::endl;
}

static bool ispass(std::string command) //限制在本文件
{
  std::vector<std::string> dict {"rm","mv","kill","while","cd"};
  for(auto &d:dict) 
  {
   if(command.find(d)!=std::string::npos) 
     return false;
  }
  return true;
}

std::string executeCommand(std::string command)
{
  if(!ispass(command))
  {
    return "command warning! permission denied";
  }

  //popen:创建管道,创建子进程,分割命令行,喂给子进程,程序替换,最后由子进程将结果的字符串带回给主进程
  //参数1:命令行字符串(自动分割),参数2:对结果"w","r","a",...
  //返回值:将结果写到文件中,返回文件指针

  FILE *fd = popen(command.c_str(),"r");
  if(fd == nullptr)
  {
    std::cout<<"execute command error"<<std::endl;
    pclose(fd);
    return "None";
  }
  std::string result;
  char buffer[2048];
  while(nullptr != fgets(buffer,sizeof(buffer),fd))
  {
    result+=buffer;
  }
  //fgets(buffer,sizeof(buffer),fd); //fgets自动在末尾添加\0.最多读n-1 
  pclose(fd);
  
  return result;
}

std::string conversionString(std::string request)
{
//将接收到的数据,小写转大写  
  std::string result;
  char c = '\0';
  for(auto r:request)
  {
    if(islower(r))
    {
      c = toupper(r) ;
      result.push_back(c);
    }
    else
    {
      result.push_back(r);
    }
  }
  //gcc不支持strlwr or strupr,不是C标准库的函数
    return result;
}

int main(int argc , char*argv[])  //接收命令行数据
{
  if(argc != 2 )
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
    return 0;
  }
  uint16_t port = atoi(argv[1]); //atoi的a是ascii码转int
  //std::unique_ptr<UdpServer> usvr(new UdpServer("1.1.1.1",port)); //虚拟机或物理机器版本
  //std::unique_ptr<UdpServer> usvr(new UdpServer(conversionString,port)); //云服务器不需要手动提供IP
  std::unique_ptr<UdpServer> usvr(new UdpServer(executeCommand,port)); 
  std::cout<<"server run"<<std::endl;
  usvr->InitServer();
  usvr->Start();

  return  0;
}
