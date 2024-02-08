#include"util.hpp"
#include"config.hpp"

namespace cloud = ns_cloud_backup;

void FileUtilTest(std::string filename)
{
  //ns_cloud_backup::FileUtil fu(filename);
  //std::cout<<fu.FileName()<<std::endl;
  //std::cout<<fu.FileSize()<<std::endl;
  //std::cout<<fu.LastATime()<<std::endl;
  //std::cout<<fu.LastMTime()<<std::endl;

  //ns_cloud_backup::FileUtil fu(filename);
  //fu.CreateDirectory();
  //
  //std::vector<std::string> v;
  //fu.ScanDirectory(v);
  //for(std::string &s:v)
  //{
  //  std::cout<<s<<std::endl;
  //}
  
}


void JsonUtilTest()
{
  const char* name = "xiaoming";
  int age = 18;
  float score[] = {85,88.5,99};
  Json::Value root;
  root["name"] = name;
  root["age"] = 18;
  root["score"].append(score[0]);
  root["score"].append(score[1]);
  root["score"].append(score[2]);
  std::string json_str;
  cloud::JsonUtil::Serialize(root,&json_str);
  std::cout<<json_str<<std::endl;  

  Json::Value res;
  cloud::JsonUtil::UnSerialize(json_str,&res);
  std::cout<<res.toStyledString()<<std::endl; 
}

void ConfigTest()
{
  cloud::Config* con = cloud::Config::GetInstance();
  std::cout<<con->GetArcSuffix()<<std::endl
    <<con->GetBackupDir()<<std::endl
    <<con->GetBackupFileName()<<std::endl
    <<con->GetHotTime()<<std::endl
    <<con->GetPackDir()<<std::endl
    <<con->GetServerIP()<<std::endl
    <<con->GetServerPort()<<std::endl
    <<con->GetUrlPrefix()<<std::endl;
}

int main(int argc, char* argv[])
{
  //FileUtilTest(argv[1]);
  //JsonUtilTest();
  ConfigTest();

  return 0;
}
