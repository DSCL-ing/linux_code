#include"server.hpp"
#include<iostream>
#include<memory>

int main()
{
  std::unique_ptr<UdpServer> usvr(new UdpServer());
  std::cout<<"server run"<<std::endl;
  usvr->InitServer();
  usvr->Start();

  return  0;
}
