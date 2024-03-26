#include<iostream>
#include<memory>
#include"server_epoll.hpp"

int main()
{
    std::unique_ptr<EpollServer> svr(new EpollServer);
    svr->InitServer();
    svr->Start();
    return 0;
}