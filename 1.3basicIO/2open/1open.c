#include<stdio.h>
#include<string.h>
#include<errno.h>

//int open
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

//int close
#include<unistd.h>

#define LOG "log.txt"

int main()
{
    //自定义掩码
    umask(0000);
   
    //只写+创建+清空
    //int fd = open(LOG,O_WRONLY|O_CREAT|O_TRUNC,0666);
    
    //追加+写入+创建:必须要有写入,追加只是到结尾,C语言是追加写入
    //int fd = open(LOG,O_APPEND|O_WRONLY|O_CREAT,0666);
    
    //只读
    int fd = open(LOG,O_RDONLY,0666);
    
    if(fd == -1) 
        printf("fd: %d,errno: %d,errstring: %s\n",fd,errno,strerror(errno));
    else
        printf("fd: %d,errno: %d,errstring: %s\n",fd,errno,strerror(errno));

    char buffer[1024];
    int n = read(fd,buffer,sizeof(buffer)-1);//使用系统IO时,注意增加或去掉\0,读出来一般是给高级语言使用的,所以至少最后一个位置要留给\0,以防溢出
    //这里只能全部读入,因为没有任何处理,C语言是处理过才能按行读入,处理方法则是一个一个读,读到换行就停止.即可实现按行读入
    if(n>0)
    {
        buffer[n] = '\0';
        printf("%s\n",buffer);
    }
    else

    //const char *msg = "aaaaaaaaaaaa";
    //int cnt = 5;
    //while(cnt)
    //{
    //    char line[128];
    //    //填充式写入
    //    snprintf(line,sizeof(line),"%s, %d\n",msg,cnt);
    //    //只取有效字符,不带隐藏字符,取有效字符长度strlen
    //    //因为系统不认识c语言的符号规则,如\0.但认识\n
    //    write(fd,line,strlen(line));
    //    --cnt;
    //}
    
    close(fd);
    return 0;
}
