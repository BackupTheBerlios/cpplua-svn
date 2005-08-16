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

// LuaProxyEmptyTable

LuaProxyEmptyTable::LuaProxyEmptyTable(LuaState* L)
: LuaIObject(L) {
}

void LuaProxyEmptyTable::push() const {
  getState()->newTable();
}

};
