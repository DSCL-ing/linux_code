

#include<iostream>
#include<pthread.h>
#include<unistd.h>  
#include<cstring>
#include<cerrno>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* routine(void*)
{
    std::cout<<"I am a new thread" <<"  ";
    
    pthread_mutex_lock(&mutex);
    std::cout<<"I got a mutex!" <<std::endl;
   
    pthread_mutex_lock(&mutex);
    std::cout<<"I am a new thread" <<"  ";
    std::cout<<"I alive again" << std::endl;
   
    return nullptr;
}

int main()
{
    pthread_t tid;
    int n = pthread_create(&tid,nullptr,routine,nullptr);
    if(n!=0)
    {
        std::cout<<"phtread_create error"<<std::endl;
        std::cout<<errno<<" : " <<strerror(errno)<<std::endl;
    }

    sleep(3);
    std::cout<<"main thread run begin" <<std::endl;
    pthread_mutex_unlock(&mutex);
    std::cout<<"main thread unlock" <<std::endl;
    sleep(3);
    

    return 0;
}
