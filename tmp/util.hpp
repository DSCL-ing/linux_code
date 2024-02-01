#pragma once 

#include<iostream>
#include<fstream>
#include<string>
#include<vector>

#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

namespace ns_cloud_backup
{
  class FileUtil
  {
    private:
      std::string _filename;//文件名称
    public:
      FileUtil(const std::string& filename):_filename(filename){}
      int64_t FileSize()
      {
        struct stat st;
        if(stat(_filename.c_str(),&st)<0)
        {
          std::cout<<"get file size error" <<std::endl;;
          return -1;
        }
        return st.st_size;

      } //返回文件大小,类型int64,能防止文件过大,并且能使用-1等作为错误码
      time_t LastMTime() //文件最后修改时间
      {
        struct stat st;
        if(stat(_filename.c_str(),&st)<0)
        {
          std::cout<<"get file last modfly time error" <<std::endl;;
          return -1;
        }
        return st.st_mtime;
      }
      time_t LastATime() //文件最后访问时间
      {
        struct stat st;
        if(stat(_filename.c_str(),&st)<0)
        {
          std::cout<<"get file last access time error" <<std::endl;;
          return -1;
        }
        return st.st_atime;
      }
      const std::string FileName() //返回文件名
      {
        //1. 路径格式为./dir/file.txt  .从最后一个'/'开始就是文件名
        //size_t pos = _filename.rfind('/');
        size_t pos = _filename.find_last_of('/'); //最后一个'/'
        if(pos == std::string::npos) //没找到说明直接就是文件名(当前目录)
        {
          return _filename;
        }
        return _filename.substr(pos+1);

      }

      bool GetPosLen(const std::string *body,size_t pos,size_t len)
      {
        std::ifstream ifs;
        ifs.open(_filename,std::ios::binary); //以二进制方式打开,稳定
          //不建议以文本方式,有些字符不一定占一个字节
        if(ifs.is_open() == false)
        {
          std::cout<<"open file failed!"<<std::endl;
          return false;
        }
        size_t fsize = FileSize();
        if(pos+len>fsize)
        {
          std::cout<<"length out of file size"<<std::endl;
          return false;
        }

        return true; 

      }



  };
}
