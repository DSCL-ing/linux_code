
#pragma once

#include<iostream>
#include<string>
#include<unistd.h>

#define NUM 1024

const std::string fifoname = "./fifo"; //当前路径的名问fifo的文件

uint32_t mode = 0666; //八进制

// mode_t; //就是unsigned int == uint32_t 
// uint32_t; // usigned int
// uint64_t; // usigned long int
// int64_t   //long int 或 long long int
// int32_t   //int
// int16_t   //short
// int8_t    //char
// size_t    // usigned long  int
// ssize_t   // signed long int
