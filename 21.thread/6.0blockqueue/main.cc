
#include "blockQueue.hpp"
#include <ctime>
#include <unistd.h>
#include "task.hpp"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer(void *args)
{
  BlockQueue<Data_t> *bq = static_cast<BlockQueue<Data_t> *>(args);
  std::string opers = "+-*/%";

  while (true)
  {
    // sleep(1);
    // 1.先通过某种渠道获取数据

    // Data_t  == int
    //  Data_t data = rand() % 10 + 1;

    // Data_t = Task
    int x = rand() % 10;
    int y = rand() % 10;
    char op = opers[rand() % opers.size()];
    // Data_t data = new Data_t(x,y,op[rand()%5]);
    Data_t data(x, y, op);
    // 2.将数据推送blockqueue中 -- 完成生产过程
    bq->push(data); //拷贝构造

    // Data_t  == int
    //  std::cout << " preducer data: " << data << std::endl;

    // Data_t = Task
    std::cout << pthread_self() << " | producer Task: " << data.formatArg() << "?" << std::endl;
  }
  return nullptr;
}

void *consumer(void *args)
{
  BlockQueue<Data_t> *bq = static_cast<BlockQueue<Data_t> *>(args);
  while (true)
  {
    // sleep(1);
    Data_t data;
    // 1.先将数据从blockqueue中获取 -- 获取到了数据
    bq->pop(&data); // 取出数据
    data();
    // 2.结合某种业务逻辑,处理数据
        // type == int
    //  std::cout << " consumer data: " << data << std::endl;

        // type == Task
    std::cout << pthread_self() << " | consumer Task: " << data.formatArg() << data.formatRes() << std::endl;
  }
  return nullptr;
}

int main()
{
  srand((uint64_t)time(nullptr) ^ getpid());
  pthread_t c[2], p[3];
  BlockQueue<Data_t> *bq = new BlockQueue<Data_t>(); // 一个交易场所,超市,公共资源,
  pthread_create(&p[0], nullptr, producer, (void *)bq); // 生产者,两个角色之一
  pthread_create(&p[1], nullptr, producer, (void *)bq); // 生产者,两个角色之一
  pthread_create(&p[2], nullptr, producer, (void *)bq); // 生产者,两个角色之一
  pthread_create(&c[0], nullptr, consumer, (void *)bq); // 消费者,两个角色之一
  pthread_create(&c[1], nullptr, consumer, (void *)bq); // 消费者,两个角色之一

  pthread_join(p[0], nullptr);
  pthread_join(p[1], nullptr);
  pthread_join(p[2], nullptr);
  pthread_join(c[0], nullptr);
  pthread_join(c[1], nullptr);

  delete bq; //new的资源,用完要释放

  return 0;
}
