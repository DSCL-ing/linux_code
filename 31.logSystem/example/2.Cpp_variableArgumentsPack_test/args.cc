#include<iostream>

void myPrintf(){};

template<typename T , typename ... Argss>
void myPrintf(const T& t,Argss && ... args) 
  //const &既能引用左值,又能引用右值
  //&&万能引用左值右值都能引用
{
  std::cout<<t<<" "<<sizeof ...(args) <<std::endl;
  myPrintf(std::forward<Argss>(args)...);
}


int main()
{
  myPrintf("hello","world");
  myPrintf(1,2,3);



  return 0;
}
