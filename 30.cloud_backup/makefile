#cloud:cloud.cpp bundle.cpp -lpthread


# 伪对象
.PHONY:
cloud:cloud.cpp 
	g++ $^ -o a.out -lstdc++fs -ljsoncpp -lbundle -Llib -lpthread -Iinclude

.PHONY:clean
clean:
	rm -rf a.out
