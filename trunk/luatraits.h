#ifndef LUATRAITS_H
#define LUATRAITS_H

#include "luaiobject.h"
#include "luastate.h"

namespace cpplua {

template <typename T>
struct LuaTraits {
  static void push(LuaState*, const T& obj) {
    obj.push();
    // FIXME: add LuaIObject template specialization
  }
};

// userdata specialization
template <typename T>
struct LuaTraits<T*> {
  static void push(LuaState* L, const T*& obj) {
    L->pushLightUserdata(obj);
  }
};

// int specialization
template <>
struct LuaTraits<int> {
  static void push(LuaState* L, const int& n) {
    L->pushNumber<int>(n);
  }
};

// string specialization
template<size_t size>
struct LuaTraits<char[size]> {
  static void push(LuaState* L, const char(&str)[size]) {
    L->pushString(str);
  }
};

};

#endif
