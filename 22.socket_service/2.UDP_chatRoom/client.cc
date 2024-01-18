
#include "client.hpp"
#include <thread>

void Usage(char* proc)
{
  // ./client serverip serverport 
  std::cout<<"Usage\n\t" <<proc<<" serverip  serverport\n"<<std::endl;
}

void  send()
{

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

  while(true)
  {
    //2.sendto
    //sendto会在首次发送数据的时,通过系统调用替用户自动选择和绑定套接字地址(IP和端口号)
    //
    std::string massage;
    std::cout<<"please Enter# ";
    //std::cin>>massage;
    std::getline(std::cin,massage); //遇到换行符才会结束
   
    sendto(sock,massage.c_str(),massage.size(),0,(struct sockaddr*)&server,sizeof(server));

    //3.recv
    char buffer[2048];
    struct sockaddr temp; //temp不需要清空,因为使用来接收的,接收到的数据会覆盖,不必担心
    socklen_t len = sizeof(temp);  
    int n = recvfrom(sock,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&temp,&len);
    if(n > 0)
    {
      buffer[n] = '\0';
      std::cout<<"server echo# :" << buffer<<std::endl;
    }
  }
  
  return 0;
}
