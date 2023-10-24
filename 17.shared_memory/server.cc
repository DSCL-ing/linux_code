
#include"comm.hpp"

int main()
{
  //1.获取key
 key_t k = getKey(); 
 std::cout<<"server_key:"<<toHex(k)<<std::endl;

 //2.创建共享内存
  int shmid = createShm(k,gsize);
  std::cout<<"server_shmid:"<<shmid<<std::endl;
  return 0;
}
