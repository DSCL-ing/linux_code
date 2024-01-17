
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <string>

std::string toHex(int num) 
{
	char buffer[64] ={0};
	snprintf(buffer,64,"0x%x",num);
	return buffer;
 }

 void *threadRoutine(void *args)
 {
   pthread_detach(pthread_self()); // ----------------------复合函数实现自己分离自己
   std::string name = static_cast<const char*>(args);
   int cnt = 5;
   while(cnt)
   {
     std::cout<<name<<" : "<<cnt--<<std::endl;
     sleep(1);
   }
   return nullptr;
 }
 
 int main()
 {
     pthread_t tid;
     pthread_create(&tid, nullptr, threadRoutine, (void*)"thread 1");
     sleep(10);                                                      
     
     //pthread_detach(tid); //主线程分离

     std::cout << "all thread quit" << std::endl;
 
   return 0;
 }
