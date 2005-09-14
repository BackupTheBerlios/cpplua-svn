#ifndef LUAFUNCTION_H
#define LUAFUNCTION_H

#include "luaobject.h"
#include "functioncall.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {

template <typename Function>
class LuaFunction : public LuaObject {};

template <typename RetVal>
class LuaFunction<RetVal(*)()> : public LuaObject {
public:
  LuaFunction(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()() {
    return FunctionCall<RetVal, 0>::apply(getState(), *this);
  }
};

template <typename RetVal>
class LuaFunction<RetVal(*)(LuaState*)> : public LuaObject {
public:
  LuaFunction(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()() {
    return FunctionCall<RetVal, 0>::apply(getState(), *this);
  }
};

template <typename RetVal, typename Arg1>
class LuaFunction<RetVal(*)(Arg1)>: public LuaObject {
public:
  LuaFunction(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1) {
    return FunctionCall<RetVal, 1>::apply(getState(), *this, arg1);
  }
};

template <typename RetVal, typename Arg1>
class LuaFunction<RetVal(*)(LuaState*, Arg1)> : public LuaObject {
public:
  LuaFunction(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1) {
    return FunctionCall<RetVal, 1>::apply(getState(), *this, arg1);
  }
};


template <typename RetVal, typename Arg1, typename Arg2>
class LuaFunction<RetVal(*)(Arg1, Arg2)>: public LuaObject {
public:
  LuaFunction(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1, const Arg2& arg2) {
    return FunctionCall<RetVal, 2>::apply(getState(), *this, arg1, arg2);
  }
};

template <typename RetVal, typename Arg1, typename Arg2>
class LuaFunction<RetVal(*)(LuaState*, Arg1, Arg2)>: public LuaObject {
public:
  LuaFunction(LuaState* L) : LuaObject(L) {}
  
  RetVal operator()(const Arg1& arg1, const Arg2& arg2) {
    return FunctionCall<RetVal, 2>::apply(getState(), *this, arg1, arg2);
  }
};


};

#endif
