//#include"thread_pool.cc"

#include<iostream>
#include<string>
#include<future>
#include<thread>
#include<chrono>
#include"thread_pool.h"

//定义一个异步任务



std::string fetchDataFromDB(std::string query){
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return "Data:"+query;
}

void use_async(){
  
  std::future<std::string> resultFromDB = std::async(std::launch::async,fetchDataFromDB,"Data");//异步执行,启动异步线程工作
  //std::async(std::launch::deferred); -- 只有在future::get或future::wait后才会同步执行
  std::cout<<"主线程做其他事"<<std::endl;
  std::string dbData = resultFromDB.get();  //get是阻塞操作,只有future获取到结果才会返回,get只能调用一次
  //wait也是阻塞函数,不过wait只等待任务完成,不返回结果;且能够判断多次; //如果还不需要处理结果就可以先用wait
  //还有wait_for,wait_until等..
  //返回值std::future_status;
  std::cout<<dbData<<std::endl;
}




void testThreadPool(){
  int m = 0;
  ThreadPool::instance().commit([](int& m){
      m = 1024;
      std::cout<<"inner set m is:" <<m<<std::endl;
      std::cout<<"m address is: " <<&m<<std::endl;

      },std::ref(m))  ; //std::ref实际是将m的地址取出来,然后仿函数wrapper()返回解引用地址*m从而实现"引用"的效果
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout<<"m is: "<<m<<std::endl; 
  std::cout<<"m address is: " <<&m<<std::endl;

}

int main(){
  use_async();  std::cout<<std::endl;
  testThreadPool(); 
  return 0;
}

