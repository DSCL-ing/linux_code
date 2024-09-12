#include<iostream>
#include<mysql/mysql.h>
#include<string>
#include<cassert>
#include<chrono>
#include<thread>

/*
MYSQL *mysql_real_connect(
        MYSQL *mysql,               // 输出型参数:成功则返回mysql访问句柄,错误返回NULL
        const char *host,           // mysqld的ip
        const char *user,           // 登录的用户名
        const char *passwd,         // 登录密码
        const char *db,             // 连接的数据库
        unsigned int port,          // mysqld端口号
        const char *unix_socket,    // 域间套接字(类似进程间管道通信)
        unsigned long clientflag);  // 位图,启用一些特性;一般使用默认就好,即0
 */

const std::string host = "127.0.0.1";
const std::string user = "chj";
const std::string passwd = "123456";
const std::string db = "test_db";
const unsigned int port = 3306;
//const std::string socket = ;
const unsigned long clientflag = 0;


int main(){
  //0. 测试mysql开发包是否正常
  std::cout<<"mysql client version :" << mysql_get_client_info()<<std::endl;
  
  //1. 初始化
  MYSQL* mysql = mysql_init(nullptr);
  if(mysql == nullptr){
    std::cerr<<" mysql init error "<<std::endl;
    return 1;
  }
  
  //2. 获取句柄
  if(mysql_real_connect(mysql,host.c_str(),user.c_str(),passwd.c_str(),db.c_str(),port,nullptr,clientflag)){
    std::cout<<"conect success" <<std::endl;
  }
  else{
    std::cerr<<"mysql connect error"<<std::endl;
    return 2;
  }

  //3. 配置链接属性[可选]
  mysql_set_character_set(mysql,"utf8");


//  std::this_thread::sleep_for(std::chrono::seconds(10));

  std::string sql;
  while(true){
    std::cout<<"mysql >>> ";
    if(!std::getline(std::cin,sql)) break;
    if(sql == "quit" || sql == "exit") break;
    int n = mysql_query(mysql,sql.c_str());
    if(n == 0){
      std::cout<<sql<<" sucess"<<std::endl;
    }
    else{
      std::cerr<<sql<<" failed, "<<n<<std::endl;
    }
    
  }


  mysql_close(mysql);

  return 0;
}
