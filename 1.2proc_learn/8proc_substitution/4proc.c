#include<stdio.h>
#include<unistd.h>
#include<sys/types.h> //getpid
#include<sys/wait.h> //wait
#include<stdlib.h>  //exit , putenv

int main()
{
    // 查看exec的返回值
    pid_t id = fork();
    if(id == 0)
    {
        printf("我是子进程: %d\n",getpid());
//C++11要求字符串常量带const,不然会警告,警告是编译器的问题,C语言不受影响,无视即可
       //  char * myargv[]={
       //     "ls",
       //     "-a",
       //     "-l",
       //     "-n",
       //     NULL
       // };
       //1.execl//   int n = execl("/usr/bin/ls","ls","-a","-l",NULL);
       //2.execv//   int n = execv("/usr/bin/ls",myargv);
       //3.execlp//  int n = execlp("ls","ls","-a","-l","-n",NULL);
       //4.execvp//  int n = execvp("ls",myargv);
       // char *const myenv[] = {  //const是函数形参需要
       //     "MYENV=我是传给C++程序的环境变量",
       //     NULL
       // };
       
        //传自定义的环境变量
        //int n = execle("./test_CPP_proc/a.outProc_Cpp","a.outProc_Cpp",NULL,myenv);
        extern char **environ;
        //传系统的环境变量
        
        putenv("MYENV=我是传给C++程序的环境变量");
        
        int n = execle("./test_CPP_proc/a.outProc_Cpp","a.outProc_Cpp",NULL,environ); 
        printf("看到我说明execl执行失败了:返回值:%d\n",n);
        exit(1);
    }
    sleep(1);
    printf("我是父进程: %d\n",getpid());
    int status = 0;
    waitpid(id,&status,0);
    printf("child exit code: %d\n",WEXITSTATUS(status));
    return 0;
}
