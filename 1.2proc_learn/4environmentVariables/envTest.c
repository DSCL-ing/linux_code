

#include<stdio.h>
#include<unistd.h>
int main()
{
  extern char** environ;
  for(int i = 0; environ[i];++i)
  {
    printf("envp[%d]->%s\n",i,environ[i]);
  }

    return 0;
}


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
