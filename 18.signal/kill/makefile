
.PHONY:all
all:mykill loop

mykill:kill.cc
	g++ -o $@ $^ -std=c++11

loop:looper.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f mykill
	rm -f loop
