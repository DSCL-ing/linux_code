#cloud:cloud.cpp bundle.cpp -lpthread

cloud:cloud.cpp 
	g++ $^ -o $@ -lstdc++fs -ljsoncpp

.PHONY:clean
clean:
	rm -rf cloud 
