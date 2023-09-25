#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define ONE    0x1  // 0000 0001    
#define TWO    0x2  // 0000 0010
#define THREE  0x4  // 0000 0100
#define FOUR   0x8  // 0000 1000            
#define FIVE   0x10 // 0001 0000

//  00000000 00000000 00000000 00000000 

void Print(int flags)
{
  if(flags & ONE )  printf("hello 1\n");
  if(flags & TWO)  printf("hello 2\n");
  if(flags & THREE)  printf("hello 3\n");
  if(flags & FOUR)  printf("hello 4\n");
  if(flags & FIVE)  printf("hello 5\n");

}



int main()
{
    Print(ONE);
    printf("--------------------------\n");
    Print(TWO|ONE);
    printf("--------------------------\n");
    Print(THREE|ONE|TWO);
    printf("--------------------------\n");
    Print(FOUR|THREE|ONE|TWO);
    printf("--------------------------\n");
    Print(FIVE|FOUR|THREE|ONE|TWO);
    printf("--------------------------\n");
    return 0;
}
