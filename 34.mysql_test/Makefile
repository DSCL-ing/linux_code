

.PHONY: test
.PHONY: clean

N=test

$(N):test.cc
	g++ -o $@ $^ -std=c++11 -L /lib64/mysql -l mysqlclient -lpthread

clean:
	rm -rf $(N)
