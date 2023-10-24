
#include"comm.hpp"

int main()
{
  //1.获取key
  key_t k = getKey();
  std::cout<<"client_key:"<<k<<std::endl;

  //2.获取共享内存
  int shmid = getShm(k,gsize);
  std::cout<<"client_shmid:"<<shmid<<std::endl;

  return 0;
}
