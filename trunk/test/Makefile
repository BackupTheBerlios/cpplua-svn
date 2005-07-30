# CppLua test makefile
# see copyright notice in luastate.h

RDCUTC="../rdcut/rdcut.rb"
CC=g++
CFLAGS=-g -ggdb -Wall -I/usr/include/lua50
LDFLAGS=-L.. -lcpplua
RDCUTSRC=$(wildcard *.rdcut)
RDCUTINC=$(wildcard *.rdcinc)
SRC=$(RDCUTSRC:%.rdcut=%.cpp)
OBJ=$(SRC:%.cpp=%.o)
TARGET=test

all: check

check: $(TARGET)
	LD_LIBRARY_PATH=".." ./$(TARGET)

clean:
	rm -f *.o *~ *.cpp

distclean:
	rm -f *.o *.so .*.dep *~ *.s $(TARGET) depend

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

%.cpp : %.rdcut $(RDCUTINC)
	$(RDCUTC) $< > $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

