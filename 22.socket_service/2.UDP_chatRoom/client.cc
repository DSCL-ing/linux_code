
#include "client.hpp"
#include <thread>

void Usage(char* proc)
{
  // ./client serverip serverport 
  std::cout<<"Usage\n\t" <<proc<<" serverip  serverport\n"<<std::endl;
}

void Send(int sock,sockaddr_in peer )
{
  while(true)
  {
    std::string massage;
    std::cout<<"please Enter# ";
    //std::cin>>massage;
    std::getline(std::cin,massage); //遇到换行符才会结束
    sendto(sock,massage.c_str(),massage.size(),0,(struct sockaddr*)&peer,sizeof(peer));
  }
}

void Recv(int sock)
{
  while(true)
  {

    char buffer[2048];
    sockaddr_in temp;
    socklen_t len;
    int n = recvfrom(sock,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&temp,&len);
    if(n > 0)
    {
      buffer[n] = '\0';
      std::cerr << buffer<<std::endl;//使用cerr
      //两个输出流,cerr流向命名管道,让消息分开
      //注意执行重定向命令后,会阻塞等待另一个终端打开管道文件
      //命令后要带2>fifo,因为默认是标准输出1.使用标准错误来复习
    }
  }
}

int main(int argc ,char* argv[])
{
  if(argc != 3)
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }
  
  std::cout << "client run" << std::endl;
  
  //1. 创建套接字,打开网络文件
  int sock = socket(AF_INET,SOCK_DGRAM,0);
  if(sock<0)
  {
    std::cerr<<"create socket error "<<std::endl;
    exit(SOCKET_ERR);
  }
 
  //1.5 准备工作
  std::string serverip = argv[1];
  uint16_t serverport = atoi(argv[2]);
  
  //1.6明确server
  struct sockaddr_in server;
  memset(&server,0,sizeof(server)); //string.h
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(serverip.c_str());
  server.sin_port = htons(serverport);
  
  std::thread t1(Send,sock,server);
  std::thread t2(Recv,sock);

  t1.join();
  t2.join();
  
  return 0;
}
