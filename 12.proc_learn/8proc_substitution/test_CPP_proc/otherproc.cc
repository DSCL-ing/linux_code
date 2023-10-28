#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
using std::cout;
using std::endl;
using std::cin;


int main()
{
    for(int i = 0 ; i<5; ++i)
    {
        cout<<"-----------------------------------------------------------"<<endl;
        cout<<"我是一个C++程序"<<" ";
        cout<<"pid:"<< getpid() <<endl;
        cout<<"MYENV:"<<(getenv("MYENV")==NULL?"NULL":getenv("MYENV"))<<endl;
        cout<<"PATH:"<<(getenv("PATH")==NULL?"NULL":getenv("PATH"))<<endl;
        sleep(1) ;
    }
    return 0;
}
