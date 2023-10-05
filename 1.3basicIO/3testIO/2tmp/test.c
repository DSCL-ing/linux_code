#include<stdio.h>
#include<unistd.h>//close
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define LOG_NORMAL "logNormal.txt"
int main()
{
    umask(0000);
    printf("fljgkagkalj\n");
    fprintf(stderr,"hello wordl!\n");

    return 0;
}
