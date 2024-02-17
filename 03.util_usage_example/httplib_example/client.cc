#include"httplib.h"

#define server_ip "8.134.236.217"
#define server_port 8080


int main()
{
  httplib::Client client(server_ip,server_port);

  struct httplib::MultipartFormData item; 
  item.name = "file";
  item.filename = "hello.txt";
  item.content = "Hello World!";
  item.content_type = "text/plain";
  
  httplib::MultipartFormDataItems items;
  items.push_back(item);

  auto res = client.Post("/multipart",items);
  std::cout<<res->status<<std::endl;
  std::cout<<res->body<<std::endl;

}
