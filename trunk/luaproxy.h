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

template <typename T>
class LuaProxyPrimitive : public LuaIObject {
  T data;
public:
  LuaProxyPrimitive(LuaState* L, const T& data);
  virtual void push() const;
};

template <typename T>
LuaProxyPrimitive<T>::LuaProxyPrimitive(LuaState* L, const T& data)
: LuaIObject(L) 
, data(data) {}

template <typename T>
void LuaProxyPrimitive<T>::push() const {
  LuaTraits<T>::push(getState(), data);
}

};

#endif
