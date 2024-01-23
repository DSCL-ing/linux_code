
#include"server.hpp"
#include"daemon.hpp"

#include<memory>

using namespace ns_server;

void Usage(std::string proc)
{
  std::cout<<"Usage:\n\t"
           <<proc<<" port"<<std::endl;
}

std::string echo(const std::string &massage)
{
  return massage;
}

int main(int argc,char *argv[])
{
  std::unique_ptr<TCPServer> tsvr;
  if(argc>2)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }
  else if(argc == 2)
  {
    tsvr = std::unique_ptr<TCPServer>{new TCPServer(echo,atoi(argv[1]))};
  }
  else
  {
    tsvr = std::unique_ptr<TCPServer>{new TCPServer(echo)};
  }
  tsvr->initServer();
  //守护进程化
  Daemon(); //一般在初始化完成后,守护进程化
  tsvr->start(); //然后以守护进程的方式启动服务器


  return 0;
}
