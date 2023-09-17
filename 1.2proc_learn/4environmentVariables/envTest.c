#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define OWNER "chj"
int main()
{
    char* user = getenv("USER");   
    if(!user) perror("getenv");
    if(!strcmp(user,OWNER))  printf("合法\n");    
    else printf("当前用户%s是非法用户\n",user);
    return 0;
}


//#include<stdio.h>
//#include<unistd.h>
//int main()
//{
//  extern char** environ;
//  for(int i = 0; environ[i];++i)
//  {
//    printf("envp[%d]->%s\n",i,environ[i]);
//  }
//    return 0;
//}


// 1.显示传参方式接受main参数,使用main参数打印环境变量
//#include<stdio.h>
//int main(int argc,char* argv[],char* envp[])
//{
//
//  for(int i = 0; envp[i];++i)
//  {
//    printf("envp[%d]->%s\n",i,envp[i]);
//  }
//  return 0;
//}
