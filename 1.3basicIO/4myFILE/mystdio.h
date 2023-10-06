#pragma once

#include<assert.h>
#include<unistd.h>
#include<malloc.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define NUM 1024
#define BUFF_NONE 0x01
#define BUFF_LINE 0x02
#define BUFF_ALL  0x04

typedef struct _MY_FILE
{
    /* hello world */
    int fd ;
    int flags; //flush method;
    char outputbuffer[NUM];
    int current; //下次写入的位置/最后写入位置的下一个
} MY_FILE;

int my_fflush(MY_FILE *stream);

size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream);

MY_FILE *my_fopen(const char *path, const char *mode);

int my_fclose(MY_FILE *stream);
