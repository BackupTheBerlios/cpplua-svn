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

LuaObject indexTable(LuaTable table, LuaObject key) {
  return table[key];
}

LUA_EXPORT int luaopen_test2(lua_State* l) {
  lua_newtable(l);
  return 1;
}

#if 0
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
#endif

int main(int argc, char** argv) {
  ofstream* stream = 0;
  if (argc > 1) {
    stream = new ofstream(argv[1]);
    LuaState::setLoggerStream(stream);
  }

  try
  {
    LuaState L;

    L.reg("f", indexTable);
    
    L.doFile("test.lua");
  }
  catch(cpplua_error error) {
    cout << error.what() << endl;
  }
  
  delete stream;
  return 0;
}
