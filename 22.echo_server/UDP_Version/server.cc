#include"server.hpp"
#include<memory>
#include<string>
#include<ctype.h>
#include<string.h>

void Usage(std::string proc)
{
  std::cout<< "Usage:\n\t" << proc <<" port\n"<<std::endl;
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
  std::unique_ptr<UdpServer> usvr(new UdpServer(conversionString,port)); //云服务器不需要手动提供IP
  std::cout<<"server run"<<std::endl;
  usvr->InitServer();
  usvr->Start();

  return  0;
}
