#ifndef __DATA_HPP__
#define __DATA_HPP__

#include"util.hpp"
#include"config.hpp"

namespace ns_cloud_backup{
  struct BackupInfo{

    bool arc_flag; //是否被压缩
    size_t fsize;  //原文件大小
    time_t mtime;  //last modify修改时间
    time_t atime;  //last access访问时间
    std::string real_path; //实际路径
    std::string arc_path;  //压缩包路径: ./压缩包路径/文件.压缩包后缀名
    std::string url;  //下载资源路径
    
    bool NewBackupInfo(const std::string &realpath) // ./dir/filename
    {
      
      FileUtil fu(realpath);
      if(fu.Exists() == false) 
      {
        std::cout<<"NewBackupInfo : file not exists"<<std::endl;
        return false;
      }
      fsize = fu.FileSize();
      mtime = fu.LastMTime(); 
      atime = fu.LastATime();
     
      arc_flag = false;
      real_path = realpath;

      Config*config = Config::GetInstance();
      std::string arc_dir = config->GetArcDir();
      std::string arc_suffix = config->GetArcSuffix();
      // ./压缩包路径/a.txt.压缩包后缀名
      arc_path =arc_dir+fu.FileName()+arc_suffix;
      // /download/a.txt
      url = config->GetUrlPrefix()+ fu.FileName();

      return true;
    }

  };

}

#endif
