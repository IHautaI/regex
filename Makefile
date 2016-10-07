VPATH = lib/
CXX = clang++
CPPFLAGS = -O3 --std=c++14 -Wall
OBJS = regex.o basic.o compound.o action.o dfa.o
LDLIBS = -lstdc++


all : test dfa-test

test : $(OBJS)

dfa-test : $(OBJS)

clean: 
	rm regex.o basic.o compound.o action.o test dfa.o dfa-test
