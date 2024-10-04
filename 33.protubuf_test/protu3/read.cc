#include<iostream>
#include<fstream>
#include<memory>

#include"contacts.pb.h"

void PrintContacts(const contacts::Contacts& con){
  for(int i = 0;i<con.peopleinfo_size() ;i++){
    contacts::PeopleInfo pi = con.peopleinfo(i);
    std::cout<<"联系人姓名: "<<pi.name()<<std::endl;
    std::cout<<"联系人年龄: "<<pi.age()<<std::endl;
    for(int j = 0 ; j<pi.phone_size();j++){
      //const contacts::PeopleInfo_Phone& phone = pi.phone(j);
      std::cout<<"联系人电话"<<j+1<<": "<<pi.phone(j).number()<<std::endl;
    }
    std::cout<<std::endl;
  }
}

int main(){
  std::fstream ifs("contacts.bin",std::ios::in|std::ios::binary);
  if(!ifs.is_open()){
    std::cerr<<"file open error"<<std::endl; 
    return -1;
  }
  contacts::Contacts con;
  con.ParseFromIstream(&ifs);
  PrintContacts(con);
  
  ifs.close();
  return 0;
}


