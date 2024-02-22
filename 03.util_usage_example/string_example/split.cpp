#include<iostream>
#include<string>
#include<vector>

int Split(const std::string &str,const std::string &sep,std::vector<std::string> *arry)
{
  //1.围绕find和substr,将字符串分割成一个个元素
 
  int count = 0; 
  size_t idx = 0,pos = 0;
  while(idx<str.size())
  //while(true) //死循环好控制,减少考虑越界,
  {
    pos = str.find(sep,idx);//find(要查找的子串,查找的起始位置)
    if(pos == std::string::npos) //找不到了,说明是最后一段,在循环外执行最后的插入 "SEP str"
    {
      break; 
    }
    if(idx == pos) // 防止因为连续的分隔符而插入空白子串 "... str SEP SEP str ..."
    {
      idx=pos+sep.size();
      continue;
    }
    
    std::string substr = str.substr(idx,pos-idx);//substr(起始位置,子串长度);
    arry->push_back(substr);
    idx = pos+sep.size();
    count++;
  }

  if(idx<str.size()) //不能插入的情况:最后一段后面还带分隔符 -- "str SEP"
  {
    arry->push_back(str.substr(idx));//substr(起始位置);从起始位置开始截取到字符串末尾
    count++;
  }
  return count;
}


int main()
{
  std::string str = "abc\ncde\n\nfgh\na";
  std::vector<std::string> v;
  Split(str,"\n",&v);
  for(auto &e:v)
  {
    std::cout<<e<<std::endl;
  }
  return 0;
}

