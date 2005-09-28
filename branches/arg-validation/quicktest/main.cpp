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

int test(int x, const char* y) {
  cout << "I'm here!!" << endl;
  if (y)
    return x;
  else
    return 0;
}

const char* constant() {
  return "all your base are belong to us";
}

int main(int argc, char** argv) {
  ofstream* stream = 0;
  if (argc > 1) {
    stream = new ofstream(argv[1]);
    LuaState::setLoggerStream(stream);
  }

  try
  {
    auto_ptr<LuaState> L(new LuaState);

    LuaFunction<LuaObject(*)(LuaState*, LuaObject, LuaObject)> f = L->function(nt);
    LuaObject t = f(L->primitive(3), L->primitive(4));
    
  }
  catch(cpplua_error e) {
    cerr << "ratta!" << endl;
    cerr << e.what() << endl;
  }

  
  delete stream;
  return 0;
}
