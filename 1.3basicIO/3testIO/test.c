#include<stdio.h>


int main()
{
    printf("%d\n",stdin->_fileno);
    printf("%d\n",stdout->_fileno);
    printf("%d\n",stderr->_fileno);
    FILE *fp = fopen("log.txt","w");
    printf("%d\n",fp->_fileno);
    return 0;
}
