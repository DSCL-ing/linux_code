
#include"server.hpp"
#include<memory>

using namespace ns_server;

void Usage(std::string proc)
{
  std::cout<<"Usage:\n\t"
           <<proc<<" port"<<std::endl;
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
    tsvr = std::unique_ptr<TCPServer>{new TCPServer(atoi(argv[1]))};
  }
  else
  {
    tsvr = std::unique_ptr<TCPServer>{new TCPServer()};
  }
  tsvr->initServer();
  tsvr->start();

    return 0;
}
