#ifndef LUAMETHOD_H
#define LUAMETHOD_H

#include "luaobject.h"
#include <stdlib.h>

namespace cpplua {

template <typename T, typename Function>
class LuaMethod : public LuaObject {};

template <typename T, typename RetVal>
class LuaMethod<T, RetVal(T::*)()> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()() {
    push();
    getState()->pcall(0, 1, 0);
    return LuaTraits<RetVal>::pop(getState());
  }
};

template <typename T, typename RetVal>
class LuaMethod<T, RetVal(T::*)(LuaState*)> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()() {
    push();
    getState()->pcall(0, 1, 0);
    return LuaTraits<RetVal>::pop(getState());
  }
};

template <typename T, typename RetVal, typename Arg1>
class LuaMethod<T, RetVal(T::*)(Arg1)> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1) {
    push();
    LuaTraits<Arg1>::push(getState(), arg1);
    getState()->pcall(1, 1, 0);
    return LuaTraits<RetVal>::pop(getState());
  }
};

template <typename T, typename RetVal, typename Arg1>
class LuaMethod<T, RetVal(T::*)(LuaState*,Arg1)> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1) {
    push();
    LuaTraits<Arg1>::push(getState(), arg1);
    getState()->pcall(1, 1, 0);
    return LuaTraits<RetVal>::pop(getState());
  }
};

};

#endif
