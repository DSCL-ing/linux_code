
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <string>

//只支持内置类型,语言级别的类型,类不支持
//只类似全局变量,不能是static,因为static一定在全局已初始化数据区
 __thread int g_val = 100;//gcc/g++的选项,使全局变量给每个线程都拷贝一份,是属于线程内部的全局变量

// int g_val = 100;

 std::string hexAddr(pthread_t tid)
 {
     g_val++; //作用:统计该函数被该线程调用了多少次
     char buffer[64];
     snprintf(buffer, sizeof(buffer), "0x%lx", tid);

     return buffer;
 }

 void *threadRoutine(void* args)
 {
     // static int a = 10;
   std::string name = static_cast<const char*>(args);
     int cnt = 5;
     while(cnt)
     {
         // cout << name << " : " << cnt-- << " : " << hexAddr(pthread_self()) << " &cnt: " << &cnt << endl;
       std::cout << name << " g_val: " << g_val++ << ", &g_val: " << &g_val << std::endl;
         sleep(1);
     }
     return nullptr;
 }

 int main()
 {
     pthread_t t1, t2, t3;
     pthread_create(&t1, nullptr, threadRoutine, (void*)"thread 1"); // 线程被创建的时候，谁先执行不确定！
     pthread_create(&t2, nullptr, threadRoutine, (void*)"thread 2"); // 线程被创建的时候，谁先执行不确定！
     pthread_create(&t3, nullptr, threadRoutine, (void*)"thread 3"); // 线程被创建的时候，谁先执行不确定!

     pthread_join(t1, nullptr);
     pthread_join(t2, nullptr);
     pthread_join(t3, nullptr);

     // pthread_detach(tid);

     // while(true)
     // {
     //     cout << " main thread:  " << hexAddr(pthread_self()) << " new thread id: " << hexAddr(tid) << endl;
     //     sleep(1);
     // }

     // if( 0 != n )
     // {
     //     std::cerr << "error: " << n << " : " <<  strerror(n) << std::endl;
     // }

     // sleep(10);
     return 0;
 }
