#include<iostream>
#include<mutex>

//C++通用版本

class Singleton
{
  public:
    static Singleton* getInstance()
    {
      if(_instance==nullptr)
      {
        std::lock_guard<std::mutex> lg(_mtx);
        if(_instance == nullptr)
        {
          _instance = new Singleton();
        }
      }
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
    //不能在堆栈上申请
    int _data;
    static std::mutex _mtx;
    static Singleton* _instance; //满足动态内存管理要求,需要使用指针
};
Singleton* Singleton::_instance = nullptr; //static成员必须在类外初始化
std::mutex Singleton::_mtx;

int main()
{
  std::cout<<"function main is started"<<std::endl;
  std::cout<<Singleton::getInstance()->getData()<<std::endl;
  return 0;
}

