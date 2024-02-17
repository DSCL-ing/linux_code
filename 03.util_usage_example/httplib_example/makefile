
.PHONY:all
all:server client

server:server.cc
	g++ $^ -o $@ -lpthread 

client:client.cc
	g++ $^ -o $@ -lpthread 

.PHONY:clean
clean: 
	rm -rf client server
