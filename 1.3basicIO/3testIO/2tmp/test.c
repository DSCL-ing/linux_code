#include<stdio.h>
#include<unistd.h>//close
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define LOG_NORMAL "logNormal.txt"
int main()
{
    umask(0000);


    return 0;
}
