
# $@: 表示要生成的目标(不一定真生成,只是语义是这样,具体生成看执行的命令)
# $^: 所有的依赖文件 $<: 第一个依赖文件

## makefile 以递归的方式(所谓依赖),找到最终的依赖条件开始执行,逐步回归

CXX := g++
TARGET1 = client
TARGET2 = server 
OBJ1 = client.cc
OBJ2 = server.cc

CXXFLAGS = -Wall -o #Warnning all 显示所有的警告

.PHONY:all
all:$(TARGET1) $(TARGET2)

$(TARGET1):$(OBJ1)
	$(CXX) $(CXXFLAGS) $(TARGET1) $< 

$(TARGET2):$(OBJ2)
	$(CXX) $(CXXFLAGS) $(TARGET2) $< 

#	这是个笛卡尔积,但没关系,上层需要哪条才用哪条
#%.o:%.cc  # "%" 表示取出来文件的文件名（不包含后缀,删除最后一个.之后的）
#	$(CXX) $(CXXFLAGS) $@ $< 

.PHONY:clean  #伪目标,不检查. 如果真目标则会检查目标是否已生成,已是最新等等... (由目标衍生出来的用法)
clean:
	rm -rf server client


