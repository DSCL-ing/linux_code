#include<iostream>

class Singleton
{
  public:
    static Singleton& getInstance() { return _instance; } 
    int getData() { return _data;}

  private:
    Singleton():_data(99)//一般情况单例的数据都是从配置文件读取
    {
      std::cout<<"Hungry Mode Singleton is Initialized!"<<std::endl;
    }; 
    Singleton(const Singleton& s)=delete;
    Singleton& operator=(const Singleton& s)=delete;
    ~Singleton(){};

  private:
    int _data;
    static Singleton _instance ; //不允许非static成员,会无限定义下去.static只允许初始化一次可以使用
};

Singleton Singleton::_instance; 
//类是一个类型,必须从代码区调用出来才算实例化


int main()
{
  std::cout<<"function main is started"<<std::endl;
  std::cout<<Singleton::getInstance().getData()<<std::endl;
  return 0;
}



