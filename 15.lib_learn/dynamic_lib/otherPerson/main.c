
#include<stdio.h>
#include"add.h"
#include"sub.h"



int main()
{
    int x = 100;
    int y = 34;
    printf("%d + %d = %d\n",x,y,add(x,y));
    printf("%d - %d = %d\n",x,y,sub(x,y));
    return 0;
}
