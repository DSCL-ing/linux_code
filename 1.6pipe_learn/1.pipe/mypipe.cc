#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>
#include<sys/types.h>
#include<sys/wait.h>
#include<cerrno>
int main()
{
    int pipefd[2] ={0}; 
    
    //1.创建管道
    int n = pipe(pipefd);

    std::cout<<"pipefd[0]:"<<pipefd[0]<<std::endl; //打印管道读端
    std::cout<<"pipefd[1]:"<<pipefd[1]<<std::endl; //打印管道写端
    
    if(n<0) //  管道打开错误 输出 
    {
        std::cout<<"pipe() error,errno: "<<errno << "," << strerror(errno)<<std::endl;
        return 1;
    }
    
   
    //2.创建子进程
    int id = fork();
    if(id < 0 ) 
    {
        //
        std::cout<<"fork() error,errno: "<<errno << "," << strerror(errno)<<std::endl;
        return 1;
    }
    else if(id == 0)
    {
        //子进程
        //关闭写端fd,只读
        close(pipefd[1]);


        //通信结束,关闭文件和结束进程
        close(pipefd[0]); //读端也关闭,
        exit(0);
    }

    //父进程
    
    //3.创建通信条件 -- 关闭读端,只写
    close(pipefd[0]);
   
    
    //4.开始通信 -- 结合某种场景...略



    int status = 0;
    waitpid(id,&status,0);

    return 0;
}
