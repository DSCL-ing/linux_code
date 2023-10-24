
// 该头文件作用:使用同一份头文件,创建共性内存

//适度封装库代码,可以更好控制程序逻辑


#ifndef __COMM_HPP__ // 如果没有定义__XX__,则执行内部代码

//ifndef内部代码
#define __COMM_HPP__ //一般配合ifndef使用,未定义则定义,下次不会再执行..

#include<iostream>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<cerrno>
#include<cstring>
#include<string>

#define PATHNAME "."
#define PROJID 0x6666   //任意值

const int gsize = 4096;

//1.获取key
key_t getKey()
{
    key_t k = ftok(PATHNAME,PROJID);
    if(k == -1)
    {
      std::cout<<"errno: "<<errno<<"-->"<<strerror(errno)<<std::endl;
      exit(1);
    }
    return k;
}

//1.1
std::string toHex(int x) //打印成string字符串
{
  char buffer[64];
  snprintf(buffer,sizeof buffer,"0x%x",x);
  return buffer;
}

//2.创建共享内存Jk


// static修饰函数:限定作用域于当前文件中
static int getShmHelper(key_t key,size_t size,int flag)//helper:辅助用
{
  int shmid = shmget(key,size,flag);
  if(shmid == -1)
  {
    std::cout<<"errno: "<<errno<<"-->"<<strerror(errno)<<std::endl;
    exit(2);
  }
  return shmid;
}

//提供给创建者
int createShm(key_t key,int size)
{
  return getShmHelper(key,size,IPC_CREAT|IPC_EXCL);
}

//提供给获取者
int getShm(key_t key,int size)
{
  return getShmHelper(key,size,IPC_CREAT);
}


//static函数-->只在本文件内有效

//相似代码复用方法


#endif//ifndef和ifdef的结束符
