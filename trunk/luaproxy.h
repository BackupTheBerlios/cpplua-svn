#ifndef LUAPROXY_H
#define LUAPROXY_H

#include "luaiobject.h"

namespace cpplua {

class LuaState;
template <typename Table, typename Key> class LuaBracket;

class LuaProxyGlobal : public LuaIObject {
  const char* name;
private:
  template <typename T>
  void assign(const T& other);
public:
  LuaProxyGlobal(LuaState* L, const char* name);
  virtual void push() const;
  
  LuaProxyGlobal& operator=(const LuaProxyGlobal& other);
  template <typename T>
  LuaProxyGlobal& operator=(const T& other);
};

template <typename T>
void LuaProxyGlobal::assign(const T& other) {
  getState()->pushString(name);
  LuaTraits<T>::push(getState(), other);
  getState()->setTable(LUA_GLOBALSINDEX);
}

template <typename T>
LuaProxyGlobal& LuaProxyGlobal::operator=(const T& other) {
  assign(other);
  return *this;
}

/**********/

class LuaProxyEmptyTable : public LuaIObject {
public:
  LuaProxyEmptyTable(LuaState* L);
  virtual void push() const;
};

/**********/

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

/**********/

class LuaProxyMetatable : public LuaIObject {
  const LuaIObject& table;
public:
  LuaProxyMetatable(LuaState* L, const LuaIObject& table);
  virtual void push() const;
  
  template <typename T>
  LuaBracket<LuaProxyMetatable, T> operator[](const T& key) const {
    return LuaBracket<LuaProxyMetatable, T>(getState(), *this, key);
  }
};

};

#endif
