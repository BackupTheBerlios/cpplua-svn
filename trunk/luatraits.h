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
  
  static LuaIObject pop(LuaState* L) {
    LuaObject res;
    L->pushLightUserdata(&res);
    L->pushValue(-2);
    L->setTable(LuaState::cpptableIndex);
    L->pop();
    return res;
  }
};

// userdata specialization
template <typename T>
struct LuaTraits<T*> {
  static void push(LuaState* L, T* const & obj) {
    L->pushLightUserdata(obj);
  }
  static T* pop(LuaState* L) {
    T* res = L->template toUserdata<T>();
    L->pop();
    return res;
  }
};

// int specialization
template <>
struct LuaTraits<int> {
  static void push(LuaState* L, const int& n) {
    L->pushNumber<int>(n);
  }
  static int pop(LuaState* L) {
    int res = L->toNumber<int>();
    L->pop();
    return res;
  }
};

// double specialization
template <>
struct LuaTraits<double> {
  static void push(LuaState* L, const double& n) {
    L->pushNumber<double>(n);
  }
  static double pop(LuaState* L) {
    double res = L->toNumber<double>();
    L->pop();
    return res;
  }
};

// string specialization
template<size_t size>
struct LuaTraits<char[size]> {
  static void push(LuaState* L, const char(&str)[size]) {
    L->pushString(str);
  }
  static char* pop(LuaState* L) {
    char* res = L->toString();
    L->pop();
    return res;
  }
};

struct LuaTraits<const char*> {
  static void push(LuaState* L, const char* const & str) {
    L->pushString(str);
  }
  static const char* pop(LuaState* L) {
    const char* res = L->toString();
    L->pop();
    return res;
  }
};

};

#endif
