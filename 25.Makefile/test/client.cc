#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<cstring>
#include<netinet/in.h>
#include<ctime>
#include<sys/time.h>

const static std::string defaultip = "192.168.1.47" ;
//const static std::string defaultip = "127.0.0.1" ;
const static uint16_t defaultport = 9999;

int main()
{
  int sock = socket(AF_INET,SOCK_STREAM,0);

  
  sockaddr_in server;
  memset(&server,0,sizeof(server));
  server.sin_port = htons(defaultport);
  inet_aton(defaultip.c_str(),&server.sin_addr);
  server.sin_family = AF_INET;
 
  connect(sock,(sockaddr*)&server,sizeof(server));
  char buffer[1024]; 
  while(true)
  {
    ssize_t n = recv(sock,buffer,sizeof(buffer),0) ;
    timeval tv; 
    gettimeofday(&tv,nullptr);
    if(n>0)
    {
      buffer[n] = 0;
      std::cout<<tv.tv_usec <<"--------"<<buffer<<std::endl;
    }
    else
    {
      break;
    }

    
  }

  return 0;
}
