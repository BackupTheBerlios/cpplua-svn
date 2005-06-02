# CppLua Makefile
# see copyright notice in luastate.h

CC=g++
CFLAGS=-g -ggdb -Wall -I/usr/include/lua50
LDFLAGS=-ldl -llua50 -llualib50
SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)
DEPS=$(SRC:%.cpp=.%.dep)
TARGET=libcpplua.so

all: $(TARGET)
	cd test; $(MAKE) $@

clean:
	-rm -f *.o *.so *~
	cd test; $(MAKE) $@

distclean:
	-rm -f *.o *.so .*.dep *~ *.s $(TARGET) depend

$(TARGET): $(OBJ)
	$(CC) -shared -o $@ $(OBJ) $(LDFLAGS)

%.o : %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.%.dep: %.cpp
	$(CC) -MM $(CFLAGS) $< >$@

-include $(DEPS)
