
#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>


  static int count = 0;

void handler(int signu)
{
  std::cout<<"get a signal: " <<signu <<std::endl;
  std::cout<<"count: "<<count<<std::endl;
  alarm(1);//自举 --- 不太明白
//  exit(14);
}

int main()
{

  //signal(2,handler);
  //signal(3,handler);
  signal(14,handler);
  
  //1秒钟运算多少次?将一个整数累计到多少次? --> 算力
  alarm(1);


 // while(true) ++count;

  while(true)
  {
    //IO,网络感觉不影响,因为数据发送后,是在本地跑完再发送数据的
    //std::cout << "count: "<<count++<<std::endl;//错误,IO占用太多
  }
  std::cout<<"count: "<<count<<std::endl;
  return 0 ;
}
