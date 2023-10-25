
#include "comm.hpp"
#include <unistd.h> //用户使用了sleep需要
int main()
{
  // 1.获取key
  key_t k = getKey();
  std::cout << "server_key:" << toHex(k) << std::endl;

  // 2.创建共享内存
  int shmid = createShm(k, gsize);
  std::cout << "server_shmid:" << shmid << std::endl;

  sleep(5);

//practice permission for. 练习权限用
  // struct shmid_ds ds;
  // int n = shmctl(shmid, IPC_STAT, &ds);
  // if (n != -1)
  // {
  //   std::cout << "perm: " << toHex(ds.shm_perm.__key) << std::endl;
  //   std::cout << "creater pid: " << ds.shm_cpid << " : " << getpid() << std::endl;
  // }

  //3.关联 共享内存
  char* start = attachShm(shmid);

  //4.通信

  //5.去关联

  //6. 删除共享内存
  delShm(shmid);
  std::cout << "removed shm,shmid:" << shmid << std::endl;
  
  return 0;



}
