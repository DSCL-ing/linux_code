#include<iostream>
#include<string>
#include<unistd.h>
#include<cassert>
#include<vector>
#include"Task.hpp"
#include<cstdlib>
#include<sys/types.h>
#include<sys/wait.h>

using namespace std;

const int gnum = 5;//创建管道的数量  -- 代替宏的全局常量


Task task;

class endPoint 
{
private:
  static int number;
public:
  int _child_id;
  int _write_fd;
  std::string processname;
public:
  endPoint(int id,int fd) : _child_id(id) , _write_fd(fd)
  {
    char namebuffer[64]; 
    //命名格式: process-0[pid:fd]
    snprintf(namebuffer,sizeof(namebuffer),"processname-%d[%d,%d]",number++,_child_id,_write_fd);
    processname = namebuffer;//刷写缓冲
  }
  const std::string name() const
  {
    return processname;
  }
};
int  endPoint::number = 0; //防止命名冲突,带类型又带类域

void WaitCommand()
{
  while(true)
  {
    int command;
    int n = read(0,&command,sizeof(int));
    if(n == sizeof(int)) task.Execute(command);
    else if(n == 0) 
    {
      std::cout<<"收到父进程的结束命令,结束进程,pid: "<<getpid()<<std::endl;
      break;//没命令了
    }
    else break; //异常
  }
}

void createProcess(std::vector<endPoint> &end_points)
{ 
  std::vector<int> fds; //fd的复数形式
  for(int i = 0; i<gnum; ++i)
  {
    //1.创建管道
    int pipefd[2] = {0};
    int n = pipe(pipefd);
    assert(n == 0); (void)n;

    //2.创建子进程
    pid_t id = fork();
    assert(id != -1);


    if(id == 0)//子进程
    {
      for(const auto& fd:fds){
        close(fd);//关闭继承自父进程的连接到其他子进程的写端
      }
      close(pipefd[1]);
      
      //希望子进程从标准输入中读取,可做可不做,练习重定向使用
      //好处是不用传参给WaitCommamd
      dup2(pipefd[0],0);

      //然后子进程开始等待,从标准输入中获取命令
      WaitCommand(); //循环执行任务,直到进程终止

      //结束收尾
      close(pipefd[0]);
      exit(0);
    }
    
    //父进程:
    close(pipefd[0]);//关闭不要的fd

    //记录父进程控制的管道写端
    fds.push_back(pipefd[1]);

    //父进程只需要有进程id和对应的管道写端即可:
    end_points.push_back(endPoint(id,pipefd[1])); 
  }

}

int ShowBoard()
{
  std::cout<<"#######################################" <<std::endl;
  std::cout<<"# 0.执行日志任务  1.执行数据库任务    #" <<std::endl;
  std::cout<<"# 2.执行请求任务  3.退出              #" <<std::endl;
  std::cout<<"#######################################" <<std::endl;
  std::cout<<"请选择# ";
  int command = 0;
  std::cin >> command; 
  return command;
}

  //父进程管理子进程 , 用户交互式
void ctrlProcess(std::vector<endPoint> *end_points)
{
  //父进程管理子进程 , 用户交互式
  int cnt = 0;
  while(true){
    //1.选择任务
    int command = ShowBoard();
    if(command == 3) {
      std::cout<<"程序结束"<<std::endl;
      break;
    }
    else{
      std::cout<<"命令不存在,请重新输入"<<std::endl;
      continue;
    }
    //2.选择进k程
    int index = cnt++; 
    cnt%=end_points->size();
    std::cout<< "选择了进程: "<<(*end_points)[index].name() << " ,处理任务"<<command<<std::endl;
    //3.下发任务
    write((*end_points)[index]._write_fd,&command,sizeof(command));
    sleep(1);

    //vector不是数组,不能使用arr+i的方式
  }
}

void waitProcess(std::vector<endPoint> &end_points)
{
  //1.要让使用管道的子进程退出,只需要让父进程关闭所有的write_fd就可以
  //2.父进程回收子进程的僵尸状态


//3.逆向写法:由管道进程复制逻辑,关闭最后一个,次低个就会正常,依次往上...
//    可以说也是取巧方法 -- 对于不标准的进程池来说
// for(int end = end_points.size()-1; end>=0;--end){
//     close(end_points[end]._write_fd);
//     std::cout<<"父进程让子进程"<<getpid()<<"退出"<<std::endl;
//     sleep(2);
//     waitpid(end_points[end]._child_id,nullptr,0);
//     std::cout<<"父进程回收了所有子进程"<<std::endl;
// }

//2.正向写法:一个接一个关闭,如果不是标准的进程池,则可能会卡住:
//  必须是关闭所有无用的fd的进程池才可以
  for(const auto &ep:end_points) {
    close(ep._write_fd);
    std::cout<<"父进程让子进程"<<ep._child_id<<"退出"<<std::endl;
    waitpid(ep._child_id,nullptr,0);
    std::cout<<"父进程回收子进程"<<ep._child_id<<std::endl;
  }


//1.巧合写法:先全部关闭后再全部回收,这样也能实现关闭全部写端
/* 
  //1.要让使用管道的子进程退出,只需要让父进程关闭所有的write_fd就可以
  //  严格来说应该是关闭一个子进程的全部写端,才能把这个子进程关闭
  for(const auto &ep:end_points) close(ep._write_fd);
  std::cout<<"父进程让所有的子进程退出"<<std::endl;
  sleep(2);
  //2.父进程回收子进程的僵尸状态
  for(const auto &ep:end_points) waitpid(ep._child_id,nullptr,0);
  std::cout<<"父进程回收了所有子进程"<<std::endl;
*/
}


//构建一个多管道进程控制程序
int main()
{

  std::vector<endPoint> end_points;
  //构建基本结构,父进程通过管道控制子进程,父进程写,子进程读
  createProcess(end_points);


  //父进程管理子进程 , 用户交互式
  ctrlProcess(&end_points);

  //处理子进程退出
  waitProcess(end_points);

  return 0;
}
