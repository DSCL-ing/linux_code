
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <string>

#define NUM 10

enum
{
  OK = 0,
  ERR
};

// 不只是可以传整型,还可以传类,传更多信息,让线程去完成
class ThreadData
{
public:
  ThreadData(const std::string &name, int id, time_t createTime, int top)
      : _name(name), _id(id), _createTime((uint64_t)createTime), _status(OK), _top(top), _result(0)
  {
  }

public:
  std::string _name;
  int _id;
  uint64_t _createTime;

  // 要返回的数据,方式1
  int _status; // 退出码
  int _top;    // 最大值
  int _result;
  // char arr[n] //如果需要线程排序,可以传数组...
};

// 也可以定义一个专门返回数据的类,方式二
// class ResultData{};

void *thread_run(void *args) // void* 可以接受任意类型,返回值和参数匹配,目的是使用同样的参数输入和输出
{
  ThreadData *td = static_cast<ThreadData *>(args);
  for (int i = 1; i <= td->_top; i++)
  {
    td->_result += i;
  }
  std::cout << td->_name << " cal done!" << std::endl;

  // sleep(1);
  // pthread_exit(td); //return和texit(pthread)返回效果一样,都是输出型参数
  return td;
}

int main()
{
  pthread_t tids[NUM];
  for (int i = 0; i < NUM; ++i)
  {
    // char* tname = new char[64];//这时就不再new了,因为new了还要另外回收
    char tname[64];
    snprintf(tname, 64, "thread-%d", i + 1);
    ThreadData *td = new ThreadData(tname, i + 1, time(nullptr), 100 + 5 * i); // 让线程做求和
    pthread_create(tids + i, nullptr, thread_run, (void*)td);
    sleep(1);
  }

  void *retval = nullptr; // 用于接收线程任务的"返回值"--- return和t_exit的

  for (int i = 0; i < NUM; ++i)
  {
    int n = pthread_join(tids[i], &retval);
    if (n != 0)
    {
      std::cout << "phread_join error" << std::endl;
    }
    ThreadData *td = static_cast<ThreadData *>(retval); //???
    if (td->_status == OK)
    {
      std::cout << td->_name
                << "计算结果是:" << td->_result << ", 它要计算的是[1," << td->_top << "]的和"
                << "\n"; //
    }
  }

  std::cout << "all thread quit" << std::endl;

  return 0;
}
