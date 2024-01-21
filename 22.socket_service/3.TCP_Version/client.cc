#include<iostream>
#include<cstring>

#include"client.hpp"
#include"err.hpp"

#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

void Usage(std::string proc)
{
  std::cout<<"Usage:\n\t"<<proc<<"serverip serverport"<<std::endl
    <<std::endl;
}

int main(int argc,char* argv[])
{
  //0.数据准备
  if(argc!=3) 
  {
    Usage(argv[0]);
    exit(USAGE_ERR);
  }
  std::string serverip = argv[1];
  uint16_t serverport = atoi(argv[2]);

  //1.创建sock
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if(sock < 0)
  {
    std::cout<<"socket create error: "<<strerror(errno)<<std::endl;
    exit(SOCKET_ERR);
  }

  //2.建立连接
  //连接失败:重连
  sockaddr_in server;
  memset(&server,0,sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(serverport);
  inet_aton(serverip.c_str(),&server.sin_addr);
  
  int cnt = 5;
  while(0 != connect(sock,(sockaddr*)&server,sizeof(server)))
  {
    sleep(1);
    std::cout<<"正在尝试重新连接,重连次数剩余:"<<cnt--<<std::endl;
    if(cnt == 0) 
    {
      break;
    }
  }
  if(cnt <= 0) //区分循环结束的条件,cnt跑完了说明超过连接失败次数
  {
    std::cout<<"socket connect error: " <<strerror(errno)<<std::endl;
    exit(CONNECT_ERR);
  }
  char buffer[1024]; 
  while(true)
  {
  //3.发送数据
    std::string massage;
    std::cout<<"Enter: ";
    std::getline(std::cin,massage);
    write(sock,massage.c_str(),massage.size());
    
  //4.接收数据
    ssize_t s = read(sock,buffer,sizeof(buffer)-1);
    if(s>0)
    {
      buffer[s] = '\0';
      std::cout<<"server echo: "<<buffer<<std::endl;
    }
    else if(s == 0) 
    {
      std::cout<<"server quit "<<std::endl;
      break;//跳出循环,结束服务
    }
    else
    {
      std::cout<<"read error: "<<strerror(errno)<<std::endl;
      break;//跳出循环,结束服务
    }
  }
  close(sock);

  return 0;
}
