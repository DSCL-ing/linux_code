#include<stdio.h>
#include<string.h>
#include<errno.h>

//int open
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

//int close
#include<unistd.h>

#define LOG "log.txt"

int main()
{
    umask(0000);
    int fd = open(LOG,O_WRONLY|O_CREAT,0666);
    if(fd == -1) 
        printf("fd: %d,errno: %d,errstring: %s\n",fd,errno,strerror(errno));
    else
        printf("fd: %d,errno: %d,errstring: %s\n",fd,errno,strerror(errno));
    close(fd);
    return 0;
}
