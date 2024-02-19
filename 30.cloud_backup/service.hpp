#ifndef __SERVICE_HPP__
#define __SERVICE_HPP__


#include"include/httplib.h"
#include"config.hpp"
#include"data.hpp"

extern ns_cloud_backup::DataManager* g_dm;

namespace ns_cloud_backup
{
  class Service
  {
    public:
      Service()
      {
        Config* config = Config::GetInstance();
        _server_port = config->GetServerPort();
        _server_ip = config->GetServerIP();
        _download_prefix = config->GetUrlPrefix();

      }
      static void Hello(const httplib::Request &req,httplib::Response rsp)
      {
        rsp.set_content("hello world!","text/plain");
        rsp.status = 200;
      }
      
      bool RunModule()
      {
        _server.Post("/upload",Upload);
        _server.Get("/listshow",listShow);
        _server.Get("/",listShow);
        std::string Download_url = _download_prefix+"(.*)";
        _server.Get(Download_url,Download);
        _server.Get("/hi1",Hello);
        _server.Get("/test",[](const httplib::Request &req,httplib::Response rsp){
            rsp.set_content("hello","text/plain");
            rsp.status = 200;
            });
        std::cout<<"service start" <<std::endl;
        _server.listen("0.0.0.0",8888);
        //_server.listen(_server_ip.c_str(),_server_port);
        return true;
      }

    private:
      static void Upload(const httplib::Request &req,httplib::Response rsp)
      {
       
        //1.判断文件是否上传成功
        auto ret = req.has_file("file");
        if(ret == false)
        {
          rsp.status = 400;
          return;
        }

        //2.取文件数据
        //注意:multipart上传的文件的正文不全是文件数据,不能直接全部拷贝
        auto file = req.get_file_value("file"); 
        
        //3.把文件数据放到back_dir中
        //需要先取得文件存放路径
        std::string backup_dir = Config::GetInstance()->GetBackupDir(); 
        std::string realpath = backup_dir+FileUtil(file.filename).FileName();
        FileUtil fu(realpath);
        fu.SetContent(file.content);
        
        //4.更新文件信息
        BackupInfo bi;
        bi.NewBackupInfo(realpath);
        g_dm->Insert(bi);
      }

      static void Download(const httplib::Request &req,httplib::Response rsp)
      {

      }

      static std::string TimeToStr(time_t t)
      {
        return ctime(&t);
      }

      static void listShow(const httplib::Request &req,httplib::Response rsp)
      {
        //1.获取所有文件信息
        std::vector<BackupInfo> infos;
        g_dm->GetAll(&infos);

        //2.构建html
        std::stringstream ss;
        ss<<"<html><head><title>Download</title></head>";
        ss<<"<body><h1>Download</h1><table>";
        ss<<"<tr>";
        //遍历信息表,每一行放一个文件信息
        for(auto &info :infos)
        {
          //html不区分单双引号
          std::string filename = FileUtil(info.real_path).FileName();
          ss<<"<td><a href='"<<info.url<<"'>"<<filename<<"</a></td>";
          ss<<"<td align='right'> "<<TimeToStr(info.atime)<<" </td>";
          ss<<"<td align='right'> "<<info.fsize/1024<<"k</td>";

        ss<<"</tr>";
        }
        ss<<"</table></body></html>";
        rsp.set_content(ss.str(),"text/html");
        rsp.status = 200;
        return ;
      }

    private:
      int _server_port;
      std::string _server_ip;
      std::string _download_prefix;
      httplib::Server _server;
  };

}
#endif
