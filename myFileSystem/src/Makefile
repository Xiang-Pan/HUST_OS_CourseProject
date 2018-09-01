# A makefile
CXX = g++
CFLAGS = --std=c++11 -Wall -Wextra -g

default: main

debug: CFLAGS += -DDEBUG
debug: default 

main: main.cpp myfs.o direntry.o inode.o
	$(CXX) $(CFLAGS) -o main main.cpp direntry.o myfs.o inode.o

myfs.o: myfs.cpp myfs.hpp
	$(CXX) $(CFLAGS) -c myfs.cpp

direntry.o: direntry.cpp direntry.hpp
	$(CXX) $(CFLAGS) -c direntry.cpp

inode.o: inode.cpp inode.hpp
	$(CXX) $(CFLAGS) -c inode.cpp

clean:
	@rm -rf *.o
