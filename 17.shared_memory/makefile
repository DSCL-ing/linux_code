
.PHONY:all
all:client_shm server_shm



client_shm:client.cc
	g++ -o $@ $^ -std=c++11

server_shm:server.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f client_shm server_shm
