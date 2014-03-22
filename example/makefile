
BINS		:=singleton
CXX			:=g++
CFLAGS		:=-g -Wall

INCLUDE		:=-I../


all:$(BINS)

.cpp:
	$(CXX) $(CFLAGS) $(INCLUDE) -o $* $*.cpp 
	
clean:
	rm -rf *.o $(BINS)
