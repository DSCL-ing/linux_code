

all:write read


write:write.cc contacts.pb.cc
	g++ -o $@ $^ -std=c++11 -lprotobuf

read:read.cc  contacts.pb.cc
	g++ -o $@ $^ -std=c++11 -lprotobuf


protoc:*.proto
	protoc --cpp_out=. $^


.PHONY:clean
clean:
	## rm *.pb.* -rf
	rm test -rf
	##rm *.bin

