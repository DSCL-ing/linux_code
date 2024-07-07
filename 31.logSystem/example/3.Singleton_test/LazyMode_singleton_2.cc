#include<iostream>
#include<mutex>

//C++11版本懒汉单例模式

class Singleton
{
  public:
    static Singleton& getInstance()
    {
      static Singleton _instance; //C++11支持本地静态变量初始化时是线程安全的
      return _instance;
    }
    int getData()
    {
      return _data;
    }

  private:
    Singleton():_data(99){
        std::cout<<"Lazy Mode Singleton Initialized!"<<std::endl;
    };   
    Singleton(const Singleton& s) = delete;
    Singleton& operator=(const Singleton&s) = delete;
    ~Singleton(){};

  private:
    int _data;
};

int main()
{
  std::cout<<"function main is started"<<std::endl;
  std::cout<<Singleton::getInstance().getData()<<std::endl;
  return 0;
}

