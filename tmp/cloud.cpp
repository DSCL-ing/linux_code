#include"util.hpp"
//#include<iostream>


void FileUtilTest(const std::string& filename)
{
  //ns_cloud_backup::FileUtil fu(filename);
  //std::cout<<fu.FileName()<<std::endl;
  //std::cout<<fu.FileSize()<<std::endl;
  //std::cout<<fu.LastATime()<<std::endl;
  //std::cout<<fu.LastMTime()<<std::endl;

  ns_cloud_backup::FileUtil fu(filename);
  fu.CreateDirectory();
  
  std::vector<std::string> v;
  fu.ScanDirectory(v);
  for(std::string &s:v)
  {
    std::cout<<s<<std::endl;
  }
  return ;
}

int main(int argc, char* argv[])
{
  FileUtilTest(argv[1]);

  return 0;
}
