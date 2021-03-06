#include "luaiobject.h"
#include "luatraits.h"  
#include "luaobject.h"
#include "luaproxy.h"
#include "luaproxycall.h"

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
  return LuaProxyFunctionCall0(getState(), *this);
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

int LuaIObject::toInteger() const {
  STACK_OPERATION(int, getState()->toNumber<int>());
}

double LuaIObject::toDouble() const {
  STACK_OPERATION(double, getState()->toNumber<double>());
}

const char* LuaIObject::toString() const {
  STACK_OPERATION(const char*, getState()->toString());
}

void LuaIObject::setMetatable(const LuaIObject& mt) {
  push();
  mt.push();
  L->setMetatable();
}

LuaProxyMetatable LuaIObject::getMetatable() const {
  return LuaProxyMetatable(getState(), *this);
}

};
