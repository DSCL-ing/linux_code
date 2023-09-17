#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void Usage(char* name)
{
  printf("\nUsage: %s -[a|b|c]\n\n",name);
  exit(0);//stdlib
}
int main(int argc,char* argv[])
{
    if(argc!=2) Usage(argv[0]);
      if(!strcmp(argv[1],"-a")) printf("执行a功能\n");
      else if(!strcmp(argv[1],"-b")) printf("执行b功能\n");
      else if(!strcmp(argv[1],"-c")) printf("执行c功能\n");
      else printf("其他功能\n");
    return 0;
}
