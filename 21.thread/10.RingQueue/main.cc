#include<iostream>
#include"ringQueue.hpp"
#include<thread>
#include<vector>
#include<string>
#include<functional>
#include<ctime>

typedef int DT;

std::string ops("+-*/%");

void* produser(void*args)
{
  RingQueue<DT>* rq = static_cast<RingQueue<DT>*>(args); //类型要匹配,指针转指针
  //RingQueue<DT>* _rq = (RingQueue<DT>*)args;
 while(true)
 {
  DT data = rand(); 
  data%=100;
  rq->push(data);
  sleep(1); //生产者慢
 }
 return nullptr;
}

void* consumer(void*args)
{
  //RingQueue<DT>* _rq = static_cast<RingQueue<DT>*>(args);
  RingQueue<DT>* rq = (RingQueue<DT>*)args;
  while(true)
  {
    DT ret; 
    rq->pop(&ret);
    std::cout<< ret <<std::endl;
  }
  return nullptr;
}


int main()
{
  srand(time(nullptr)*getpid());
  RingQueue<DT> *rq = new RingQueue<DT>(); //定义在栈上也行,最后&地址传地址就好
  std::vector<std::thread> c(3); //要用圆括号,和数组不一样不能用方括号,最好用花括号,统一更好
  std::vector<std::thread> p{2}; 
  for(int i = 0; i<3;i++)
  {
    std::function<void*(void*)> fn = consumer;  //复习包装器function的用法
    //c[i] = std::thread(fn,rq);
    c.push_back(std::thread(fn,rq)); //不给vector大小时常用的方法 //如果vector给定了大小,那push_back要小心越界,因为插入的数据是在最后的,前面的几个数据可能会忽略,导致异常
  }
  
  for(int i = 0; i<2; i++)
  //for(auto&v:p)
  {
    p[i] = std::thread(produser,rq);
    //v=std::thread(consumer,rq); //未初始化vector大小时的方法
  }

  for(int i = 0; i<3;i++)
  {
    c[i].join();
  }
  for(auto&v:p)
  {
    v.join();
  }
  delete rq;
  return 0;
}
