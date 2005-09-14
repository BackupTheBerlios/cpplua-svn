#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>
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

// create a table t and set t[key] = value;
LuaObject nt(LuaState* L, LuaObject key, LuaObject value) {
  LuaObject table = L->emptyTable();
  table[key] = value;
  return table;
}

int main(int argc, char** argv) {
  ofstream* stream = 0;
  if (argc > 1) {
    stream = new ofstream(argv[1]);
    LuaState::setLoggerStream(stream);
  }

  {
    auto_ptr<LuaState> L(new LuaState);

    L->global("f") = L->function(nt);
    L->doString("t = f(5, 7); print(t); print(t[5])");

  }
  
  delete stream;
  return 0;
}
