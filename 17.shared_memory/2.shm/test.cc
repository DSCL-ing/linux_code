
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<cassert>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/ipc.h>

#define PATHNAME "."
#define PROJ_ID 0x6666
const int size = 1024;

int main()
{
  key_t key = ftok(PATHNAME,PROJ_ID);
  int shmid = shmget(key,size,IPC_CREAT|IPC_EXCL|0666)  ;
  assert(shmid!=-1); 

  char *start = (char*)shmat(shmid,nullptr,0); 

  pid_t id = fork();
  assert(id>=0);

  if(id == 0)
  {

    std::cout<<"我是子进程,向进程写入了数据"<<std::endl;
    char c = 'A';
    while(c<='Z')
    {
      start[(c-'A')] = c;
      ++c;
      start[c-'A'] = '\0';
    }
    shmdt(start);
    exit(1);
  }
  

  sleep(1);
  std::cout<<"父进程读到了:"<<start<<std::endl; 
  shmdt(start);

  shmctl(shmid,IPC_RMID,nullptr);

  waitpid(id,nullptr,WNOHANG);

  return 0;
}
