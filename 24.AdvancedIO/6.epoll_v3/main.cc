#include<iostream>
#include<memory>
#include"server_epoll.hpp"
#include<string>

/*业务逻辑*/
std::string echoServer(std::string str)
{
  //构建response
  std::string res = str;
  res+="[echo]\r\n";
  return res;
}

int main()
{
    std::unique_ptr<EpollServer> svr(new EpollServer(echoServer));
    svr->InitServer();
    svr->Dispacher();
    return 0;
}
