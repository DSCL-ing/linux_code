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


int main(int argc , char*argv[])  //接收命令行数据
{
  uint16_t port;
  if(argc > 2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
    return 0;
  }
  else if(argc == 2 )
  {
    port = atoi(argv[1]); //atoi的a是ascii码转int
    std::unique_ptr<UdpServer> usvr(new UdpServer(port)); 
    usvr->start();
  }
  else
  {
    std::unique_ptr<UdpServer> usvr(new UdpServer());
    usvr->start();
  }

  std::cout<<"server run"<<std::endl;
  return  0;

}
