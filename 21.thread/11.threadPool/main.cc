
#include<iostream>
#include"task.hpp"
#include<ctime>
#include<memory>
#include<string>
//#include"1threadpool_v1.hpp"
//#include"2threadpool_v2.hpp"
//#include"3threadpool_v3.hpp"
#include"4threadpool_v4.hpp"


std::string ops = "+-*/%";

int main()
{
  srand(time(nullptr)*getpid());
  std::unique_ptr<ThreadPool<Task>> tp(ThreadPool<Task>::getInstance()); //借助智能指针析构是否可行
  tp->init();
  tp->start();
  int cnt = 5;
  while(cnt--)
  {
    int x = rand()%100;
    int y = rand()%100;
    int op = ops[rand()%ops.size()];
    Task t( x, y ,op );
    tp->pushTask(t);
    std::cout<<t.formatArg()<<"?" <<std::endl;
    sleep(1);
  }


  return 0;
}
