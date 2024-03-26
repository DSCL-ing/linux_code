#pragma once
#include <iostream>

const static uint16_t g_port = 8888;

class EpollServer
{
public:
    EpollServer(uint16_t port):port_(port)
    {
        std::cout << std::endl;
    }
    void InitServer();
    void Start();

private:
    uint16_t port_;
};