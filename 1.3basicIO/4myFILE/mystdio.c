#include"mystdio.h"




MY_FILE *my_fopen(const char *path, const char *mode)
{
    //打开方式,以确定标志位,用于确定open打开方式
    //尝试open,并确定是否成功
    //构建MY_FILE结构体
    //初始化结构体
    //返回文件指针
    int flag = 0 ;
    if(strcmp(mode,"r")==0) flag = O_RDONLY; 
    else if(strcmp(mode,"w")==0) flag = O_WRONLY|O_CREAT|O_TRUNC; 
    else if(strcmp(mode,"a")==0) flag = O_APPEND|O_CREAT|O_TRUNC; 
    else
    
    {
        //其他选项,如"w+","a+"
    }

    mode_t m = 0666;
    int fd = 0;
    if(flag & O_CREAT) fd = open(path,flag,m);
    else fd = open(path,flag);

    if(fd<0)
    {
        perror("open fail!");
        return NULL;
    }

    MY_FILE *mf = (MY_FILE*)malloc(sizeof(MY_FILE));
    if(mf == NULL )
    {
        perror("malloc fail!");
        return NULL;
    }

    mf->fd = fd;
    mf->flags = BUFF_LINE; //刷新策略的flag
    mf->current = 0; 
    memset(mf->outputbuffer,'\0',sizeof(mf->outputbuffer)) ;//初始化器只能在定义时使用

    return mf;
}


int my_fflush(MY_FILE *stream)
{
   //立刻刷新
    assert(stream->fd); 
   //调用write
   write(stream->fd,stream->outputbuffer,stream->current) ;
   //重置信息
   stream->current = 0 ;
   
   //调用fsync阻塞等待保证冲刷成功
   fsync(stream->fd);//强制刷新到磁盘文件中
   return 0;
}

// 我们今天返回的就是一次实际写入的字节数，我就不返回个数了
size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream)
{
        // 1. 缓冲区如果已经满了，就直接写入
    if(stream->current == NUM) my_fflush(stream);

    // 2. 根据缓冲区剩余情况，进行数据拷贝即可
    size_t user_size = size * nmemb;
    size_t my_size = NUM - stream->current; // 100 - 10 = 90

    size_t writen = 0;
    if(my_size >= user_size)
    {
        memcpy(stream->outputbuffer+stream->current, ptr, user_size);
        //3. 更新计数器字段
        stream->current += user_size;
        writen = user_size;
    }
    else
    {
        memcpy(stream->outputbuffer+stream->current, ptr, my_size);
        //3. 更新计数器字段
        stream->current += my_size;
        writen = my_size;
    }
    
    // 4. 开始计划刷新, 他们高效体现在哪里 -- 
    // 不发生刷新的本质，不进行写入，就是不进行IO,不进行调用系统调用，所以my_fwrite函数调用会非常快,数据会暂时保存在缓冲区中
    // 可以在缓冲区中积压多份数据，统一进行刷新写入，本质：就是一次IO可以IO更多的数据，提高IO效率
    if(stream->flags & BUFF_ALL)
    {
        if(stream->current == NUM) my_fflush(stream);
    }
    else if(stream->flags & BUFF_LINE)
    {
        if(stream->outputbuffer[stream->current-1] == '\n') my_fflush(stream);
    }
    else
    {
        //其他刷新策略:
        //TODO
    }
    return writen;

}


int my_fclose(MY_FILE *stream)
{
    //断言 -- 帮助调试
    //刷新缓冲区
    //关闭文件 , 安全
    //释放堆空间
    //置空 -- 虽然没什么用,free已经做了,好习惯
    assert(stream);
    my_fflush(stream);
    close(stream->fd);
    free(stream);
    stream = NULL;
    return 0;
}
