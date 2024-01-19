#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma warning(disable:4996)

#pragma comment(lib, "ws2_32.lib")

uint16_t serverport = 8888;
std::string serverip = "47.113.229.137";

// easyx ege
int main()
{
    WSADATA WSAData;

    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        std::cerr << "init error" << std::endl;
        return -1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "create socket error" << std::endl;
        exit(-2);
    }
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    while (true)
    {
        std::string message;
        std::cout << "Please Enter Your Message# "; // 2
        //std::cin >> message;

        std::getline(std::cin, message);
        sendto(sock, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));


        char buffer[2048];
        struct sockaddr_in temp;
        int len = sizeof(temp);
        int n = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&temp, &len);
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << buffer << std::endl; //1
        }
    }

    closesocket(sock);
    WSACleanup();

	return 0;
}