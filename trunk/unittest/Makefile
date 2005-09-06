# Copyright (c) 2005 Paolo Capriotti
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

CC=g++
CFLAGS=-g -ggdb -Wall -I/usr/include/lua50
LDFLAGS=-L.. -lcpplua -lcppunit
DEBUG=-D_DEBUG
SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)
DEPS=$(SRC:%.cpp=.%.dep)
TARGET=test

all: $(TARGET)

clean:
	-rm -f *.o *.so *~

distclean:
	-rm -f *.o *.so .*.dep *~ *.s $(TARGET) depend

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

%.o : %.cpp
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $<

.%.dep: %.cpp
	$(CC) -MM $(CFLAGS) $< >$@

-include $(DEPS)
