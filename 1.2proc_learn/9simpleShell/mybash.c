#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/types.h>
#include<wait.h>
#define MAX 1024
#define ARGC 64
#define SEP " " //Separator
int putenv();
int split(char*commandStr,char*argv[])
{
    //strtok的写法
    argv[0]=strtok(commandStr,SEP);
    if(argv[0] == NULL) return -1;//保证不是空串,因为argc[1]也==NULL,两个NULL可能会留下隐患
    int i =1;
    while((argv[i++] = strtok(NULL,SEP)));
    return 0;
}
void showEnv()
{
    extern char** environ;
    for(int i = 0;environ[i];++i) printf("%d:%s\n",i,environ[i]);
}
void debugPrint(char*argv[])
{
    for(int i = 0; argv[i];i++)
    {
        printf("%s\n",argv[i]);
    }
}
int main()
{
    int last_exit_code = 0;
    while(1)
    {
        char myenv[32][256];//环境变量的内容
        int env_index = 0;//行号
        char commandStr[MAX]= {0};
        char *argv[ARGC]={NULL};
        printf("[mybash@host_name]$ ");
        fflush(stdout);
        char*s = fgets(commandStr,sizeof(commandStr)-1,stdin);//读n-1,第n个自动加\0.但是,有时为了统一,可以-1,少一个空间影响不大,能更好的和系统调用对齐
        assert(s);
        (void)s; //假装已经使用了这个变量,就不会再报错了
        commandStr[strlen(commandStr)-1] = '\0';//消除fgets最后读入的换行
        //分割命令行字符串
        int n = split(commandStr,argv);//封装strtok
        if(n != 0) continue;
        //细节补充
        if(strcmp(argv[0],"cd")==0)
        {
            if(argv[1]!=NULL) chdir(argv[1]);
            continue;
        }
        else if(strcmp(argv[0],"export")==0)
        {
            if(argv[1]!=NULL)
            {//单独保存添加的环境变量的内容,不然下个循环就会覆盖掉
                strcpy(myenv[env_index],argv[1]);
                putenv(myenv[env_index]);
                ++env_index;
            }
            continue;
        }
        else if(strcmp(argv[0],"env")==0)
        {
            showEnv();
            continue;
        }
        else if(strcmp(argv[0],"echo")==0)
        {
            char *target_env = NULL;
            if(*argv[1]=='$')
            {
                if(argv[1][1] == '?')
                {
                    printf("%d\n",last_exit_code);
                    last_exit_code = 0;
                    continue;
                }
                else target_env = getenv(argv[1]+1);
            }
            if(target_env!=NULL) printf("%s:%s\n",argv[1]+1,target_env);
            continue;
        }
        if(strcmp(argv[0],"ls")==0)
        {
            int pos = 0;
            while(argv[pos]) ++pos;
            argv[pos++] = (char*)"--color=auto";
            argv[pos] = NULL;//安全点,虽然已经全局初始化成NULL
        }
        //debugPrint(argv);
        //创建子进程
        pid_t id = fork();
        if(id == 0)
        {
        //程序替换
            execvp(argv[0],argv) ;
            exit(1);
        }
        int status=0;
        waitpid(id,&status,0);
        //printf("child exit code:%d\n",WEXITSTATUS(status));
        last_exit_code = WEXITSTATUS(status);
        //sleep(100);
    }
    return 0;
}
