#include "luapredefined.h"

namespace cpplua {

LuaTable::LuaTable(LuaState* L) 
: LuaObject(L) {}

LuaTable::LuaTable(const LuaTable& other)
: LuaObject(other) {}

LuaTable::LuaTable(const LuaIObject& other)
: LuaObject(other) {

  if (other.type() != TableType)
    throw cpplua_error("Type mismatch in LuaTable construction");
}

};
