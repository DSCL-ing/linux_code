#include"thread_pool.cc"


int main(){
  ThreadPool::instance().commit([](){
      for(int i = 0 ; i<10 ; i++){
      std::cout<<"hello thread_pool"<<"\n";
      }
      });
  return 0;
}
