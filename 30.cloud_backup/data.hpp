#ifndef __DATA_HPP__
#define __DATA_HPP__

#include"util.hpp"
#include"config.hpp"
#include"pthread.h"
#include<unordered_map>
#include<vector>
#include<mutex>

namespace ns_cloud_backup{
  typedef struct BackupInfo{

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

  }BackupInfo;

  class DataManager
  {
    public:
      DataManager()
      {
        _backup_file = Config::GetInstance()->GetBackupFileName();//持久化文件
        pthread_rwlock_init(&_rwlock,nullptr);
      } 
      ~DataManager()
      {
        pthread_rwlock_destroy(&_rwlock);
      }
      bool Insert(const BackupInfo&info)
      {
        pthread_rwlock_wrlock(&_rwlock);
        _table[info.url] = info;
        pthread_rwlock_unlock(&_rwlock);
        return true;
      }
      bool Update(const BackupInfo&info) //目前和insert没有区别
      {
        pthread_rwlock_wrlock(&_rwlock);
        _table[info.url] = info; //覆盖
        pthread_rwlock_unlock(&_rwlock);
        return true;
      }
      bool GetOneByURL(const std::string& url,BackupInfo*info)//根据url获取一条info
      {
        pthread_rwlock_wrlock(&_rwlock);
        auto it = _table.find(url);
        if(it == _table.end()) 
        {
          info = nullptr;
          pthread_rwlock_unlock(&_rwlock);
          return false;
        }
        *info = it->second;
        pthread_rwlock_unlock(&_rwlock);
        return true;
      }
      bool GetOneByRealPath(const std::string realpath,BackupInfo*info)//根据realpath获取一条info
      {
        pthread_rwlock_wrlock(&_rwlock);
        for(auto it:_table)
        {
          if(it.second.real_path == realpath)
          {
            *info = it.second;
            pthread_rwlock_unlock(&_rwlock);
            return true;
          }
        }
        pthread_rwlock_unlock(&_rwlock);
        return false;
      }

      bool GetAll(std::vector<BackupInfo> *arry)//获取所有的info放入vector
      {
        pthread_rwlock_wrlock(&_rwlock);
        if(_table.empty())
        {
          pthread_rwlock_unlock(&_rwlock);
          return false;
        }
        for(auto it:_table)
        {
          arry->push_back(it.second);
        }
        pthread_rwlock_unlock(&_rwlock);
        return true;
      }

    private:
      std::string _backup_file;//持久化文件
      pthread_rwlock_t _rwlock;//读写锁,同时读,互斥写
      std::unordered_map<std::string,BackupInfo> _table; //key是url
  };

}

#endif
