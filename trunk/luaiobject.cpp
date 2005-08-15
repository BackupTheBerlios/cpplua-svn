#include "luaiobject.h"
#include "luatraits.h"  

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {
 
#define IS(something) \
  bool LuaIObject::is##something() const { \
    push(); \
    bool res = getState().is##something(); \
    getState().pop(); \
    return res; \
  }
IS(Function)
IS(Number)
IS(String)
IS(Nil)
IS(Userdata)
#undef IS

LuaType LuaIObject::type() const {
  push();
  LuaType res = getState().type();
  getState().pop();
  return res;
}

const char* LuaIObject::typeName() const {
  push();
  const char* res = getState().typeName();
  getState().pop();
  return res;
}

};
