#include "luaproxy.h"

namespace cpplua {

// LuaProxyGlobal

LuaProxyGlobal::LuaProxyGlobal(LuaState* L, const char* name)
: LuaIObject(L)
, name(name) {
}

void LuaProxyGlobal::push() const {
  getState()->getGlobal(name);
}

LuaProxyGlobal& LuaProxyGlobal::operator=(const LuaProxyGlobal& other) {
  assign(other);
  return *this;
}

// LuaProxyEmptyTable

LuaProxyEmptyTable::LuaProxyEmptyTable(LuaState* L)
: LuaIObject(L) {
}

void LuaProxyEmptyTable::push() const {
  getState()->newTable();
}

};
