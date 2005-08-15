#include "luaobject.h"

namespace cpplua {

/**
  * Create an empty object. 
  * This constructor whould only be used
  * internally by cpplua.
  */
LuaObject::LuaObject(LuaState& L)
  : LuaIObject(L) {
}

LuaObject::LuaObject(const LuaObject& obj) 
  : LuaIObject(obj.getState()) {
  getState().duplicateObject(&obj, this);
}

void LuaObject::push() const {
  getState().pushObject(this);
}


};
