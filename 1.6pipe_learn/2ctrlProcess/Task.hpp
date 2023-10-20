//.hpp文件约定允许成员方法的声明和实现不分离,能使用更少的文件,
//通常用于开源项目,易于整理

#pragma once

#include<iostream>
#include<vector>
#include<sys/types.h>
#include<unistd.h>

typedef void (*fun_t)();

void PrintLog()
{
    std::cout<<"pid" << getpid()<<"正在执行'日志打印'任务..."<<std::endl;
}

void InsertMySQL()
{
    std::cout<<"pid" << getpid()<<"正在执行'数据库'任务..."<<std::endl;
}

void NetRequest()
{
    std::cout<<"pid" << getpid()<<"正在执行'网络请求'任务..."<<std::endl;
}

//如果使用宏则必须约定是4字节(因为使用了整型) -- 配合read
enum task{COMMAND_LOG,COMMAND_MYSQL,COMMAND_REQUEST};
//宏和枚举都可以,但枚举范围只限整型,如果是连续多个整型好用.
//宏更灵活,只要控制得好即可

class Task
{
public:
    Task()//定义对象后就自动加载任务
    {
      funcs.push_back(PrintLog);    
      funcs.push_back(InsertMySQL);    
      funcs.push_back(NetRequest);    
    }
    void Execute(int command)
    {
      if(command >= 0&& command < funcs.size()) funcs[command]();
      else
      {
        std::cout<<"Execute error"<<std::endl;
      }
    }
    ~Task()
    {}

public:
    std::vector<fun_t> funcs;

};
