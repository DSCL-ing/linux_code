#include<stdio.h>
#include<stdarg.h>

void myPrint(const char * fmt,...)
{
  va_list arg;
  va_start(arg,fmt);
  vprintf(fmt,arg);
  va_end(arg);
}

int main()
{
  myPrint("%d %s\n",1,"hello");
  myPrint("");
  return 0;
}


//##__VA_ARGS__
/*
#define LOG(fmt,...) printf("[%s:%d]" fmt,__FILE__,__LINE__,##__VA_ARGS__);

int main()
{
  LOG("hello",999);
  puts("");
 
  return 0;

}
*/
