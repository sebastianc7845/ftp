CXX = g++
CXFLAGS = -std=c++11 -g -Iinclude -pedantic -Wall -Wextra -Wconversion
CXLIB = -L./lib
CXLIBS = -llibtelnet.a

all: dir client server

dir:
	mkdir -p build

server: src/srv-main.cpp build/server.o build/reply.o build/response.o build/command.o build/network.o include/common.h
	$(CXX) $(CXFLAGS) $^ -o $@

client: src/cli-main.cpp build/client.o build/reply.o build/response.o build/command.o build/network.o include/common.h
	$(CXX) $(CXFLAGS) $^ -o $@

build/client.o: src/client.cpp include/client.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@ $(CXLIB) $(CXLIBS)

build/server.o: src/server.cpp include/server.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@ $(CXLIB) $(CXLIBS)

build/response.o: src/response.cpp include/response.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@ $(CXLIB) $(CXLIBS)

build/reply.o: src/reply.cpp include/reply.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@ $(CXLIB) $(CXLIBS)

build/command.o: src/command.cpp include/command.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@ $(CXLIB) $(CXLIBS)

build/network.o: src/network.cpp include/network.hpp
	$(CXX) $(CXFLAGS) -c $< -o $@ $(CXLIB) $(CXLIBS)

clean:
	rm -f client server && rm -fr build