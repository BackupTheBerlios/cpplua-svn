#include "luaiobject.h"
#include "luatraits.h"  
#include "luaobject.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {

LuaIObject::LuaIObject(LuaState* L)
: L(L) {}

LuaIObject::LuaIObject(const LuaIObject& obj)
: L(obj.getState()) {}

LuaObject LuaIObject::operator()() {
  LuaObject res(getState());
  getState()->pushLightUserdata(&res);
  push();
  LowLevelFunctionCall::protectedCall(getState(), 0, 1);
  getState()->setTable(LuaState::cpptableIndex);
  return res;
}

#define IS(something) \
  bool LuaIObject::is##something() const { \
    STACK_OPERATION(bool, getState()->is##something()); \
  }
IS(Function)
IS(Number)
IS(String)
IS(Nil)
IS(Userdata)
IS(Table)
#undef IS

LuaType LuaIObject::type() const {
  STACK_OPERATION(LuaType, getState()->type());
}

const char* LuaIObject::typeName() const {
  STACK_OPERATION(const char*, getState()->typeName());
}

const char* LuaIObject::toString() const {
  STACK_OPERATION(const char*, getState()->toString());
}

};
