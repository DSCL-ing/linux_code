#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<cstring>
#include<string>
#include<ctime>
#include<sys/time.h>

static const uint16_t defaultport = 9999;

int main()
{
  int listensock = socket(PF_INET,SOCK_STREAM,0);
  sockaddr_in local;
  memset(&local,0,sizeof(local));
  local.sin_addr.s_addr =INADDR_ANY;
  local.sin_port = htons(defaultport);
  local.sin_family = AF_INET;
  if(bind(listensock,(sockaddr*)&local,sizeof(local))<0)
  {
    std::cout<<"bind error,errstring:"<<strerror(errno)<<std::endl;
  }

  if(listen(listensock,128) < 0)
  {
    std::cout<<"listen error"<<std::endl;
    return 0;
  }

  sockaddr_in client;
  memset(&local,0,sizeof(local));
  socklen_t len;
  int sock = accept(listensock,(sockaddr*)&client,&len);
  while(true) 
  {
    char buffer[1024];
    std::cin>>buffer;
    send(sock,buffer,sizeof(buffer),0);
    timeval tv; 
    gettimeofday(&tv,nullptr);
    std::cout<<tv.tv_usec<<" ---- "<< buffer<<std::endl;
  }

  
  return 0;
}
