#include<stdio.h>
#include<unistd.h>
#include<sys/types.h> //getpid
#include<sys/wait.h> //wait
int main()
{
    //验证子进程中的exec是否会对父进程有影响 -- 不会
    pid_t id = fork();
    if(id == 0)
    {
        printf("我是子进程: %d\n",getpid());
        execl("/usr/bin/ls","ls","-a","-l",NULL);

    }
    sleep(5);
    printf("我是父进程: %d\n",getpid());
    waitpid(id,NULL,0);
    return 0;
}
