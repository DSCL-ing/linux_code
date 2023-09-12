#include<unistd.h>
#include"prog.h"
#define synbol ' '
#define arrow '>'

void progress()
{
  const char* lable = "/-|\\";
  char bar[51]; //不能太长,会出bug
  memset(bar,'\0',sizeof(bar)); 
  int i = 0;
  for(;i<51;++i)
  {
    printf("  [%-50s][%d%%][%c]\r","",i*2,lable[i%4]);
     printf("  [\033[44m%-s\033[0m\r",bar);// /%在某些场景不兼容
     fflush(stdout);
     bar[i] = synbol; 
    // if(i!=49)
     //   bar[i+1]=arrow;
     usleep(100000);
  }
  printf("\n");
}
