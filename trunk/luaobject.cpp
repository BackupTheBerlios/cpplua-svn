#include "luaobject.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {

/**
  * Create an empty object. 
  * This constructor should only be used
  * internally by cpplua.
  */
LuaObject::LuaObject(LuaState* L)
  : LuaIObject(L) {
}

LuaObject::LuaObject(const LuaIObject& obj) 
  : LuaIObject(obj) {
  duplicate(obj);
}

LuaObject::LuaObject(const LuaObject& obj)
  : LuaIObject(obj) {
  duplicate(obj);
}

void LuaObject::push() const {
  getState()->pushObject(this);
}


};
