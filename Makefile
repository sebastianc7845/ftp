CXX = g++
CXFLAGS = -std=c++11 -g -Iinclude -pedantic -Wall -Wextra -Wconversion

all: dir client server

dir:
	mkdir -p build

server: src/srv-main.cpp build/server.o build/response.o build/command.o build/network.o include/common.h
	$(CXX) $(CXFLAGS) $^ -o $@

client: src/cli-main.cpp build/client.o build/response.o build/command.o build/network.o include/common.h
	$(CXX) $(CXFLAGS) $^ -o $@

build/client.o: src/client.cpp include/client.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@

build/server.o: src/server.cpp include/server.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@

build/response.o: src/response.cpp include/response.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@

build/command.o: src/command.cpp include/command.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@

build/network.o: src/network.cpp include/network.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@

clean:
	rm -f client server && rm -fr build