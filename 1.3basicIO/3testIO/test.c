#include<stdio.h>
#include<unistd.h>//close
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define LOG "log.txt"
int main()
{
    umask(0000);
   
   
   
  


   

    //3.关闭stdin和stderr后,可以发现0,2号位置都给了新文件 
   //即证明:文件描述符的分配规则:在文件描述符中,把最小的,没有被使用的数组元素,分配给新文件
    //close(0) ;
    //close(2) ;
    //int fd0 =  open(LOG,O_WRONLY|O_CREAT|O_TRUNC,0666);
    //int fd1 =  open(LOG,O_WRONLY|O_CREAT|O_TRUNC,0666);
    //int fd2 =  open(LOG,O_WRONLY|O_CREAT|O_TRUNC,0666);
    // 
    //int fd3 =  open(LOG,O_WRONLY|O_CREAT|O_TRUNC,0666);
    //int fd4 =  open(LOG,O_WRONLY|O_CREAT|O_TRUNC,0666);
    //int fd5 =  open(LOG,O_WRONLY|O_CREAT|O_TRUNC,0666);

    //fprintf(stdout,"%d\n",fd0);  //0
    //fprintf(stdout,"%d\n",fd1);  //3
    //fprintf(stdout,"%d\n",fd2);  //4
    //fprintf(stdout,"%d\n",fd3);  //5
    //fprintf(stdout,"%d\n",fd4);  //6
    //fprintf(stdout,"%d\n",fd5);  //7

   
    //2:关闭stdout(1)后.printf的文件打印到了log文件里
    //说明:操作系统只认识文件描述符fd,不在意fd所对应的流是谁 -- 重定向原理
   // close(1) ;
   // int fd0 =  open(log,o_wroNLY|O_CREAT|O_TRUNC,0666);
   // int fd1 =  open(log,o_wroNLY|O_CREAT|O_TRUNC,0666);
   // int fd2 =  open(log,o_wroNLY|O_CREAT|O_TRUNC,0666);
   // int fd3 =  open(log,o_wroNLY|O_CREAT|O_TRUNC,0666);
   // int fd4 =  open(log,o_wroNLY|O_CREAT|O_TRUNC,0666);
   // int fd5 =  open(log,o_wroNLY|O_CREAT|O_TRUNC,0666);
   // int fd6 =  open(log,o_wroNLY|O_CREAT|O_TRUNC,0666);

   // printf("%d\n",fd0);
   // printf("%d\n",fd1);
   // printf("%d\n",fd2);
   // printf("%d\n",fd3);
   // printf("%d\n",fd4);
   // printf("%d\n",fd5);
   // printf("%d\n",fd6);
   
    //1.验证:file中一定包含fd,调用系统IO
    //printf("%d\n",stdin->_fileno);
    //printf("%d\n",stdout->_fileno);
    //printf("%d\n",stderr->_fileno);
    //file *fp = fopen("log.txt","w");
    //printf("%d\n",fp->_fileno);
    return 0;
}
