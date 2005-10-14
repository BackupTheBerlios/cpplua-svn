 #include <iostream>
#include <fstream>
#include <cassert>
#include <memory>
#include "../cpplua.h"

using namespace std;
using namespace cpplua;

#define LUA_EXPORT extern "C"

class A {
  int a;
public:
  A(int a) : a(a) {}
  int sum(int x) const {
    return x + a;
  }
};

int test() {
  return 37;
}

int sum(int a, int b) {
  return a + b;
}

int doSum(A* x, int y) {
  return x->sum(y);
}

LuaObject createTable(LuaState* L) {
  return L->emptyTable();
}

LUA_EXPORT int luaopen_test2(lua_State* l) {
  lua_newtable(l);
  return 1;
}

LUA_EXPORT int luaopen_test(lua_State* l) {
  try {
     LuaState(l).reg("sum", sum)
                .reg("create_table", createTable)
                ;
  }
  catch(cpplua_error error) {
    cout << error.what() << endl;
    return 0;
  }
  return 1;
}

int main(int argc, char** argv) {
  ofstream* stream = 0;
  if (argc > 1) {
    stream = new ofstream(argv[1]);
    LuaState::setLoggerStream(stream);
  }

  try
  {
    LuaState L;

    LuaObject a = L["A"] = L.emptyTable();
    a.setMetatable(a);
    a["sum"] = L(mem_fun(&A::sum));
    
    A x(10);
    L["x"] = L.primitive(&x);
    
    L.doFile("test.lua");
  }
  catch(cpplua_error error) {
    cout << error.what() << endl;
  }
  
  delete stream;
  return 0;
}
