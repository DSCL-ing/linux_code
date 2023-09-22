#include<stdio.h>
#include<unistd.h>
int main()
{
    //目的:查看exec对当前进程的印象,exle的功能...
    printf("begin...\n");
    printf("begin...\n");
    printf("begin...\n");
    printf("begin...\n");
    execl("/usr/bin/ls","ls","-a","-l",NULL);
    printf("end...\n");
    printf("end...\n");
    printf("end...\n");
    printf("end...\n");
    return 0;
}
