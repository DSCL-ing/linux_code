#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>
#include<sys/types.h>
#include<sys/wait.h>
#include<cerrno>
#include<string>
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

    if(id == 0)
    {
        //子进程
        //关闭写端fd,只写
        close(pipefd[0]);
       
        //开始通信
        //示例:给父进程传输某格式信息 -- 类似网络信息 
        char buffer[1024] = {'\0'};
        const std::string str = "hello,我是子进程";//实际会限制str有效字符最多1023.留一个给\0
        int cnt = 0;
        while(true)
        {
            snprintf(buffer,sizeof(buffer),"%s, 计数器: %d, 子进程id: %d",str.c_str(),cnt++,getpid());
            int n = write(pipefd[1],buffer,strlen(buffer));//strlen不计算\0,strlen会得到不会超过1023的有效字符
            std::cout<< "成功写入的字符数: " << n<<std::endl;
            sleep(1);
        }
        //通信结束,关闭文件和结束进程
        close(pipefd[1]); //读端也关闭,因为管道也是个文件
        exit(0);
    }

    //父进程
    char buffer[1024];
    while(true) 
    {
        int n = read(pipefd[0],buffer,sizeof(buffer)-1);//由于read读的是字节流,需要sizeof-1预留一位给\0
        if( n > 0 )
        {
           buffer[n] = '\0';//第n个字符后加上\0,使其成为C字符串Jk
           std::cout<<"我是父进程,子进程的massage: "<<buffer<<std::endl;
           std::cout<<"成功读取的字符数: "<<n<<std::endl;
       }
    }
    //3.创建通信条件 -- 关闭读端,只读
    close(pipefd[1]);
   
    
    //4.开始通信 -- 结合某种场景...略


    close(pipefd[0]);//关闭管道文件
    int status = 0;
    waitpid(id,&status,0);

    return 0;
}
