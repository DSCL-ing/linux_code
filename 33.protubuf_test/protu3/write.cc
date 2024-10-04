#include<iostream>
#include<fstream>

#include"contacts.pb.h"

void Add_PeopleInfo(contacts::PeopleInfo* pi){
  std::cout << "-------------新增联系⼈-------------" << std::endl;
  std::cout << "请输⼊联系⼈姓名: ";
  std::string name;
  std::getline(std::cin,name);
  pi->set_name(name);

  std::cout << "请输⼊联系⼈年龄: ";
  int age = 0;
  std::cin>>age;
  pi->set_age(age);
  std::cin.ignore(256,'\n');  //cin读完后,缓冲区还会剩分隔符及后面的数据,如果下次读取是getline则会把这个分隔符一并读取,可能导致得到不想要的结果;因此可以清理一下

  for(int i = 0; ;i++) {
    std::cout<<"请输入联系人电话"<<i+1<<"(只输入回车完成电话新增):";
    std::string number;
    getline(std::cin,number);
    if(number.empty())//getline会忽略分割符(换行),下次会跳过
    {
      break;
    }
    pi->add_phone()->set_number(number);
  }

  std::cout<<"添加联系人成功"<<std::endl;
}


int main(){
  // GOOGLE_PROTOBUF_VERIFY_VERSION 宏: 验证没有意外链接到与编译的头⽂件不兼容的库版 本。如果检测到版本不匹配，程序将中⽌。注意，每个 .pb.cc ⽂件在启动时都会⾃动调⽤此宏。在使 ⽤ C++ Protocol Buffer 库之前执⾏此宏是⼀种很好的做法，但不是绝对必要的。 
  GOOGLE_PROTOBUF_VERIFY_VERSION; 
 

  //1. 打开文件
  //2. 反序列化已有数据并读取至内存
  //3. 追加
  //4. 序列化并写入回文件

  //1
  std::fstream ifs("contacts.bin",std::ios::in|std::ios::binary);
  if(ifs.is_open()){
    std::cout<<"contacts.bin is open"<<std::endl;
  }
  else{
    std::cout<<"contacts.bin is not found"<<std::endl;
  }

  //2
  contacts::Contacts con;
  con.ParseFromIstream(&ifs);


  //3.追加
  Add_PeopleInfo(con.add_peopleinfo());


  //4.写回
  std::fstream ofs("contacts.bin",std::ios::out|std::ios::binary);
  if(ofs){
    std::cout<<"contacts.bin is open"<<std::endl;
  }
  else{
    std::cout<<"contacts.bin is not found"<<std::endl;
  }

  con.SerializeToOstream(&ofs);

  ifs.close();
  ofs.close();

    // 在程序结束时调⽤ ShutdownProtobufLibrary()，为了删除 Protocol Buffer 库分配的所 有全局对象。对于⼤多数程序来说这是不必要的，因为该过程⽆论如何都要退出，并且操作系统将负责 回收其所有内存。但是，如果你使⽤了内存泄漏检查程序，该程序需要释放每个最后对象，或者你正在 编写可以由单个进程多次加载和卸载的库，那么你可能希望强制使⽤ Protocol Buffers 来清理所有 内容。 
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
