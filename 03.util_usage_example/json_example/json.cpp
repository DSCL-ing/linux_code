
#include<iostream>
#include<jsoncpp/json/json.h>
#include<sstream>
#include<memory>


std::string test1()
{

  const char *name = "xiaoming";
  int age = 18;
  float score[]={77.5,88,93.6};

  Json::Value root; //构建一个json格式的对象-Value类型
  root["name"] = name;
  root["age"] = 18;
  root["score"].append(score[0]);
  root["score"].append(score[1]);
  root["score"].append(score[2]);

  Json::StreamWriterBuilder swb;
  std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
  std::stringstream ss;
  sw->write(root,&ss);
  std::cout<<ss.str()<<std::endl;
  return ss.str();
}
void test2(std::string str)
{
  Json::CharReaderBuilder crb;
  std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
  
  Json::Value root;
  bool ret = cr->parse(str.c_str(),str.c_str()+str.size(),&root,nullptr);
  if(ret == false)
  {
    std::cout<<"parse error"<<std::endl;;
    return ;
  }
  std::string name = root["name"].asString();
  int age = root["age"].asInt();
  float score[root["score"].size()];
  std::cout<<name<<std::endl;
  std::cout<<age<<std::endl;
    


  for(uint i =0; i< root["score"].size();i++)
  {
      score[i] = root["score"][i].asFloat();
      std::cout<<score[i]<<std::endl;
  }
  
}

int main()
{
  std::string ret = test1();
  test2(ret);


  return 0;
}
