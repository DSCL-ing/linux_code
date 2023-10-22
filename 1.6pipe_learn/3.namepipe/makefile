CC = g++
CFLAGS =  
LDFLAGS = -std=c++11

FILE1 = server
FILE2 = client

.PHONY:all
all: server client 

server: server.cc
	$(CC) $(LDFLAGS) -o $@ $^

client: client.cc
	$(CC) $(LDFLAGS) -o $@ $^

# .PHONY:clean
clean:
	rm -f $(FILE1) $(FILE2) 

