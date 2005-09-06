#include <iostream>
#include <fstream>
#include <cassert>
#include "../cpplua.h"

using namespace std;
using namespace cpplua;

class A {
  int a;
public:
  A(int a) : a(a) {}
  int sum(int x) {
    return x + a;
  }
};

int square(int x) {
  return x * x;
}

int main(int argc, char** argv) {
  ofstream* stream = 0;
  if (argc > 1) {
    stream = new ofstream(argv[1]);
    LuaState::setLoggerStream(stream);
  }

  {
    LuaState L;
    LuaObject table = L.emptyTable();
    for(int i = 0; i < 10; i++)
      table[i] = i*i;
    table["hello"] = -1.8;
    LuaObject table2 = L.emptyTable();
    table[table2] = "A table index";
    table["x"] = table2;
    table2[0] = 6;
    
    assert(table[table["x"][0]] == 36);
  }
  
  delete stream;
  return 0;
}
