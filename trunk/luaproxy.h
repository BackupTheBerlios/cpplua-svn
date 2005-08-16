#ifndef LUAPROXY_H
#define LUAPROXY_H

#include "luaiobject.h"

namespace cpplua {

class LuaState;

class LuaProxyGlobal : public LuaIObject {
  const char* name;
public:
  LuaProxyGlobal(LuaState* L, const char* name);
  virtual void push() const;
};

class LuaProxyEmptyTable : public LuaIObject {
public:
  LuaProxyEmptyTable(LuaState* L);
  virtual void push() const;
};

};

#endif
