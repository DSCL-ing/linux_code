
#include<iostream>
#include"httplib.h"

void Hello(const httplib::Request &req,httplib::Response rsp)
{
  rsp.set_content("Hello World","text/plain");
  rsp.status = 200;
}
void Numbers(const httplib::Request &req,httplib::Response &rsp)
{
  auto num = req.matches[1];
  rsp.set_content(num,"text/plain");
  rsp.status = 200;

}

//multipart不一定是上传文件,但通常用来上传文件
void Multipart(const httplib::Request &req,httplib::Response &rsp)
{
  auto ret = req.has_file("file");
  if(ret == false)
  {
    std::cout<<"not file upload"<<std::endl;
    rsp.status = 400;  // 请求方式有问题
    return;
  }
  const auto& file = req.get_file_value("file");
  rsp.body.clear();
  rsp.body +="\n";
  rsp.body +=file.content; //文件内容
  rsp.set_header("Content-Type","text/plain");
  rsp.status = 200;
  return;
}

int main()
{
  httplib::Server server;
  
  server.Get("/hi",Hello);
  server.Get("/numbers/(//d+)",Numbers);
  server.Post("/multipart",Multipart);

  server.listen("0.0.0.0",8080);//接收所有访问ip

  return 0;

}
