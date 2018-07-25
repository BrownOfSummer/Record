.PHONY:clean all distclean
CXX=g++
CFLAGS=-O3 -g -Wall
INCLUDE=-I/opt/X11/include $(shell pkg-config --cflags opencv) -std=c++11
LIBS=$(shell pkg-config --libs opencv)
all:
	make target
target:imgutils.o
	$(CXX) $(CFLAGS) -o target imgutils.o $(LIBS)
.cpp.o:
	$(CXX) $(INCLUDE) -c $< -o $@
clean:
	-rm target
	-rm *.o

