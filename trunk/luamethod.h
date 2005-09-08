#ifndef LUAMETHOD_H
#define LUAMETHOD_H

#include <stdlib.h>
#include "luaobject.h"
#include "functioncall.h"

namespace cpplua {

template <typename T, typename Function>
class LuaMethod : public LuaObject {};

template <typename T, typename RetVal>
class LuaMethod<T, RetVal(T::*)()> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()() {
    return FunctionCall<RetVal, 0>::apply(getState(), *this);
  }
};

template <typename T, typename RetVal>
class LuaMethod<T, RetVal(T::*)(LuaState*)> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()() {
    return FunctionCall<RetVal, 1>::apply(getState(), *this, getState());
  }
};

template <typename T, typename RetVal, typename Arg1>
class LuaMethod<T, RetVal(T::*)(Arg1)> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1) {
    return FunctionCall<RetVal, 1>::apply(getState(), *this, arg1);
  }
};

template <typename T, typename RetVal, typename Arg1>
class LuaMethod<T, RetVal(T::*)(LuaState*,Arg1)> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1) {
    return FunctionCall<RetVal, 2>::apply(getState(), *this, getState(), arg1);
  }
};


template <typename T, typename RetVal, typename Arg1, typename Arg2>
class LuaMethod<T, RetVal(T::*)(Arg1, Arg2)> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1, const Arg2& arg2) {
    return FunctionCall<RetVal, 2>::apply(getState(), *this, arg1, arg2);
  }
};

template <typename T, typename RetVal, typename Arg1, typename Arg2>
class LuaMethod<T, RetVal(T::*)(LuaState*,Arg1, Arg2)> : public LuaObject {
public:
  LuaMethod(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1, const Arg2& arg2) {
    return FunctionCall<RetVal, 2>::apply(getState(), *this, getState(), arg1, arg2);
  }
};

};

#endif
