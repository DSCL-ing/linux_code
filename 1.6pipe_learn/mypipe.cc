#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>
int main()
{
    int pipefd[2] ={0}; 
    
    int n = pipe(pipefd);
    if(n<0)
    {
        std::cout<<"pipe() error,errno: "<<errno << "," << strerror(errno)<<std::endl;
        return 1;
    }

    std::cout<<"pipefd[0]:"<<pipefd[0]<<std::endl;
    std::cout<<"pipefd[1]:"<<pipefd[1]<<std::endl;
    return 0;
}
