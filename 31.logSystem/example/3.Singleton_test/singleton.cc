#include<iostream>

class Singleton
{
  public:
    static Singleton& getInstance()
    {
      return _instance;
    } 

  private:
    ~Singleton();
    Singleton();
    Singleton(const Singleton& s)=delete;
    Singleton& operator=(const Singleton& s)=delete;

  private:
    static Singleton _instance ; //不允许非static成员,会无限定义下去.static只允许初始化一次可以使用
};
Singleton Singleton::_instance; 
//类是一个类型,必须从代码区调用出来才算实例化


int main()
{

  return 0;
}



