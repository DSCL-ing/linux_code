#include<iostream>
#include"contacts.pb.h"
#include<string>


int main(){
  std::string str;
  
  {
    contacts::PeopleInfo pi;
    pi.set_age(18);
    pi.set_name("张三");
    if(pi.SerializeToString(&str)){
      std::cout<<"序列化成功,结果: "<<str<<std::endl;
    }
    else{
      std::cerr<<"序列化失败"<<std::endl;
      return -1;
    }
  }

  {
    contacts::PeopleInfo pi;
    if(pi.ParseFromString(str)){
      std::cout<<"反序列化成功 "<<std::endl;
      std::cout<<"姓名: "<<pi.name()<<std::endl;
      std::cout<<"年龄: "<<pi.age()<<std::endl; 
    }
    else{
      std::cerr<<"反序列化失败"<<std::endl;
      return -1;
    }

  }

  return 0;
}
