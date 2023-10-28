//#pragma once
#include<stdio.h>

int addToTop(int top)
{
  printf("enter addToTop\n"); 

  int sum = 0;
  for(int i = 1;i<=top;++i)
  {
    sum +=i; 
  }
  printf("quit addToTop\n");
  return sum;

}

int main()
{

  int top = 100;
  int result = addToTop(top);
  printf("result:%d\n",result);
  return 0;;
}
