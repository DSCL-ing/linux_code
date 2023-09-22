#include<stdio.h>
#include<unistd.h>
#include<sys/types.h> //getpid
#include<sys/wait.h> //wait
#include<stdlib.h>  //exit
int main()
{
    // 查看exec的返回值
    pid_t id = fork();
    if(id == 0)
    {
        printf("我是子进程: %d\n",getpid());
        //exec成功,查看程序执行错误的退出码 
        int n = execl("/usr/bin/ls","ls","bqa",NULL);
   
       // int n = execl("/usr/bin/lsssss","lsssss","-a","-l",NULL);
 //如果执行成功了,那也没必要看返回值了,因为看不到了,整个代码都被替换掉
        printf("看到我说明execl执行失败了:返回值:%d\n",n);
        exit(1);
    }
    sleep(5);
    printf("我是父进程: %d\n",getpid());
    int status = 0;
    waitpid(id,&status,0);
    printf("child exit code: %d\n",WEXITSTATUS(status));
    return 0;
}
