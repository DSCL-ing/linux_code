
#pragma once

#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdarg>
#include<string>
#include<ctime>

#include<unistd.h>
#include<sys/types.h>

//日志也是有日志等级的
//#define DEBUG 0  //用来调试的
//#define INFO 1   //正常的日志信息
//#define WARNING 2//告警,不影响运行,提醒程序员注意下的
//#define ERROR 3  //一般错误,不足以影响服务器继续向后运行,服务器还可以跑
//#define FATAL 4  //致命,非常严重

static const std::string filename = "Log/tcpserver.log";

enum
{
  DEBUG = 0,
  INFO,
  WARNING,
  ERROR,
  FATAL,
  UKNOWN //nknown
};

static std::string toLevelString(int level)
{
  switch(level)
  {
    case DEBUG:
      return "DEBUG"; //开源项目中很多是直接return,不使用break
    case INFO:
      return "INFO";
    case WARNING:
      return "WARNING";
    case ERROR:
      return "ERROR";
    case FATAL:
      return "FATAL";
    default:
      return "NKNOWN";
  }
}

static std::string getTime()
{
//struct tm
//  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
//  int tm_min;			/* Minutes.	[0-59] */
//  int tm_hour;			/* Hours.	[0-23] */
//  int tm_mday;			/* Day.		[1-31] */
//  int tm_mon;			/* Month.	[0-11] */
//  int tm_year;			/* Year	- 1900.  */
//  int tm_wday;			/* Day of week.	[0-6] */
//  int tm_yday;			/* Days in year.[0-365]	*/
//  int tm_isdst;			/* DST.		[-1/0/1]*/
 
 
  time_t curr = time(nullptr);
  struct tm *tmp = localtime(&curr); //参数事time_t类型的时间戳
  char buffer[128];
  snprintf(buffer,sizeof(buffer),"%d-%d-%d %d:%d:%d",tmp->tm_year+1900,tmp->tm_mon+1,tmp->tm_mday,tmp->tm_hour,tmp->tm_min,tmp->tm_sec);
  return buffer;
}

//日志格式: 日志等级 时间 文件 行 pid 消息体
extern void logMessage(int level,const char *format, ...)  //""是字符串常量.是右值,所以需要加const 
{
  char logLeft[1024] ; //左半部分:日志等级 时间 pid //文件和行有点麻烦, 先不做处理
  std::string level_string = toLevelString(level);
  std::string curr_time = getTime();
  snprintf(logLeft,sizeof(logLeft),"[%s] [%s] [%d] ",level_string.c_str(),curr_time.c_str(),getpid());

  char logRight[1024]; //右半部分:消息体
  va_list p;
  va_start(p,format);
  vsnprintf(logRight,sizeof(logRight),format,p);
  va_end(p);

  //打印
  printf("%s %s \n",logLeft,logRight);
 

  //或者保存到文件中,以后只需要在logMessage中修改定义就可以实现日志信息的流向
  //FILE *fp = fopen(filename.c_str(),"a") ;
  //if(fp == nullptr) 
  //{
  // return ; 
  //}
  //fprintf(fp,"%s %s \n",logLeft,logRight);
  //fflush(fp);
  //fclose(fp);

}
