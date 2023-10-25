
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
#include<cassert>
#include<sys/stat.h>

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

//static函数-->只在本文件内有效
//2.1 static修饰函数:限定作用域于当前文件中
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

//2.2提供给创建者
int createShm(key_t key,int size)
{
  umask(0);
  return getShmHelper(key,size,IPC_CREAT|IPC_EXCL|0666);
  //IPC_CREAT的八进制是1000,权限一般是0xxx,所以不会冲突
}

//2.2提供给获取者
int getShm(key_t key,int size)
{
  return getShmHelper(key,size,IPC_CREAT);
}

//3.关联共享内存,//自己创建的shm不一定是给自己用的,可能是给其他进程使用,所以也需要关联才能用
char* attachShm(int shmid)
{
  char* start = (char*)shmat(shmid,nullptr,0);//类似malloc,在虚拟地址上申请空间
  return start;
}



//.删除共享内存
void delShm(int shmid)
{
  int n = shmctl(shmid,IPC_RMID,nullptr);
  assert(n != -1);//删除不影响程序,不用强制退出,
  (void)n;
}










//相似代码复用方法


#endif//ifndef和ifdef的结束符