#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/types.h>
#include<wait.h>
#define MAX 1024
#define ARGC 64
#define SEP " "
int split(char*commandStr,char*argv[])
{
    //strtok的写法
    argv[0]=strtok(commandStr,SEP);
    if(argv[0] == NULL) return -1;
    int i =1;
    while((argv[i++] = strtok(NULL,SEP)));
    return 0;
}
void debugPrint(char*argv[])
{
    for(int i =0; argv[i];i++)
    {
        printf("%s\n",argv[i]);
    }
}
int main()
{
    while(1)
    {
        char commandStr[MAX]= {0};
        char *argv[ARGC]={NULL};
        printf("[mybash@host_name]$ ");
        fflush(stdout);
        char*s = fgets(commandStr,sizeof(commandStr),stdin);
        assert(s);
        (void)s; //假装已经使用了这个变量,就不会再报错了
        //分割命令行字符串
        commandStr[strlen(commandStr)-1] = '\0';
        int n = split(commandStr,argv);
        if(n != 0) continue;
        //debugPrint(argv);
        //创建子进程
        pid_t id = fork();
        if(id == 0)
        {
        //程序替换
            execvp(argv[0],argv) ;
            exit(1);
        }
        int status=0;
        waitpid(id,&status,0);
        printf("child exit code:%d\n",WEXITSTATUS(status));
        //sleep(100);
    }
    return 0;
}
