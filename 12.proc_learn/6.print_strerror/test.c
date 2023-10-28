#include<stdio.h>
#include<string.h>//strerror
#include<stdlib.h>//exit
#include<unistd.h>//_exit
int Sum()
{
  printf("start");
  int n = 100;
  int sum = 0;
  for(int i = 0 ; i<n; ++i )
  {
      sum +=i;
  }
  _exit(11);
  printf("end\n");
  return 0;
  
}


int main()
{
  int ret = Sum();
  for(int i = 0; i<200 ; ++i)
  {
    printf("%d : %s\n",i,strerror(i));
  }
  
  return 0;
}
