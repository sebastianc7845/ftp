# Declare locations of files with specified extension
# .h files in include directory
vpath %.hpp ./include
vpath %.h ./include
# .c(pp) files in src directory
vpath %.cpp ./src
vpath %.c ./src

# Declare C++ compiler + flags
CXX := g++
CXXFLAGS := -std=c++11 -g -Iinclude -pedantic -Wall -Wextra -Wconversion
LDFLAGS := -L./lib
LDLIBS := -llibtelnet.a

# Declare build directory
BLDDIR := ./build

# DECLARE ALL SRC FILES
SRCS = (shell find -name '*.cpp')
# DECLARE ALL OBJ FILES
OBJS = $(SRCS:%.cpp=$(BLDDIR)/%.o)
# DECLARE ALL DEP FILES
DEPS = $(OBJS:%.o=%.d)

all: dir client server

dir:
	mkdir -p $(BLDDIR)

server: build/srv-main.o build/server.o build/reply.o build/response.o build/command.o build/network.o
	$(CXX) $^ -o $@ 

client: build/cli-main.o build/client.o build/reply.o build/response.o build/command.o build/network.o
	$(CXX) $^ -o $@


# BUILD STEP FOR CPP FILES
$(BLDDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MP -MMD

clean:
	rm -f client server *.d && rm -fr build

-include $(DEPS)