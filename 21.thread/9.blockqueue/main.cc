
#include"blockQueue.hpp"
#include<ctime>
#include<unistd.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer(void *args)
{
  BlockQueue<Data_t> *bq = static_cast<BlockQueue<Data_t>*>(args);
  while(true)
  {
    //1.先通过某种渠道获取数据
    Data_t data = rand()%10+1;
    //2.将数据推送blockqueue中 -- 完成生产过程
    bq->push(data);
  }
  return nullptr;
}

void* consumer(void*args)
{
  BlockQueue<Data_t> *bq = static_cast<BlockQueue<Data_t>*>(args);
  while(true)
  {
    Data_t data;
    //1.先将数据从blockqueue中获取 -- 获取到了数据
    bq->pop(&data);//取出数据
    //2.结合某种业务逻辑,处理数据
    std::cout<<" consumer data: "<<data<<std::endl;
  }
  return nullptr;
}

Data_t main()
{
  srand((uint64_t)time(nullptr)^getpid());
  pthread_t c,p;
  BlockQueue<Data_t> *bq = new BlockQueue<Data_t>(); //一个交易场所,超市,公共资源,
  pthread_create(&p,nullptr,producer,(void*)bq);//生产者,两个角色之一
  pthread_create(&c,nullptr,consumer,(void*)bq);//消费者,两个角色之一
 


  pthread_join(p,nullptr);
  pthread_join(c,nullptr);

  return 0 ;
}
