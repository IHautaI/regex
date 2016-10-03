VPATH = lib/
CXX = clang++
CPPFLAGS = -O3 --std=c++14 -Wall
LSOBJS = basic.o compound.o
LDLIBS = -lstdc++


all : test

test : $(LSOBJS)

clean: 
	rm basic.o compound.o test
