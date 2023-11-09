
#include"comm.hpp"
#include<unistd.h>

int main()
{
  Init init(CLIENT);
  char* start = init.getStart();



  
  char c = 'A';
  while(c<='Z')
  {
    start[c-'A'] = c; // 从0开始
    ++c;
    start[c-'A'] = 0; //字符串结束
    sleep(1);
  }



  /*
  //1.获取key
  key_t k = getKey();
  std::cout<<"client_key:"<<toHex(k)<<std::endl;

  //2.获取共享内存
  int shmid = getShm(k,gsize);
  std::cout<<"client_shmid:"<<shmid<<std::endl;
  
  //3.关联 共享内存
  char* start = attachShm(shmid);

  //4.通信

  //5.去关联
  detachShm(start);

  */

  return 0;
}
