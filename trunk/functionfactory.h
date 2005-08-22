#ifndef FUNCTION_FACTORY_H
#define FUNCTION_FACTORY_H

#include <vector>
#include "luatraits.h"
#include "luastate.h"
#ifdef _DEBUG
#include <iostream>
#endif


namespace cpplua {

typedef std::vector<LuaObject> LuaCollection;
template <typename T, typename Function> struct GeneralMethod;

// metafunctions

template <typename Arg>
struct RetrieveFirstArgument {
  static Arg apply(LuaState* L) {
    return LuaTraits<Arg>::pop(L);
  }
};

template <>
struct RetrieveFirstArgument<LuaState*> {
  static LuaState* apply(LuaState* L) {
    return L;
  }
};


template <typename RetVal>
struct ReturnValues {
  static int apply(LuaState* L, const RetVal& retVal) {
    LuaTraits<RetVal>::push(L, retVal);
    return 1;
  }
};
/*
template <>
struct ReturnValues<LuaCollection> {
  static int apply(LuaState* L, const LuaCollection& collection) {
    LuaCollection::const_iterator cend = collection.end();
    for(LuaCollection::iterator i = collection.begin(); i != cend; ++i)
      LuaTraits<LuaObject>::push(*i);
    return collection.size();
  }
};*/



// Function Factory

template <typename T, typename Function>
struct FunctionFactory {
  typedef Function FType;
  static void push(LuaState* L, const T& obj, Function f);
};

template <typename T, typename Function>
void FunctionFactory<T, Function>::push(LuaState* L, const T& obj, Function f) {
  L->pushLightUserdata(const_cast<T*>(&obj));
  Function* ptmf = reinterpret_cast<Function*>(
    L->newUserdata(sizeof(Function))
  );
  *ptmf = f;
  L->pushCClosure(GeneralMethod<T, Function>::apply, 2);
}

// General Method

template <typename T, typename Function> struct GeneralMethod {};

template <typename RetVal, typename T>
struct GeneralMethod<T, RetVal(T::*)()> {
  static int apply(lua_State*) {
    LuaState L(l);
    T* obj = L.template toUserdata<T>(lua_upvalueindex(1));
    RetVal(T::*f)() = *(L.template toUserdata<RetVal(T::*)()>(lua_upvalueindex(2)));
    RetVal res = (obj->*f)();
    return ReturnValues<RetVal>::apply(&L, res);  
  }
};

template <typename RetVal, typename T, typename Arg1>
struct GeneralMethod<T, RetVal(T::*)(Arg1)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    T* obj = L.template toUserdata<T>(lua_upvalueindex(1));
    RetVal(T::*f)(Arg1) = *(L.template toUserdata<RetVal(T::*)(Arg1)>(lua_upvalueindex(2)));
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    RetVal res = (obj->*f)(arg1);
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

template <typename RetVal, typename T, typename Arg1, typename Arg2>
struct GeneralMethod<T, RetVal(T::*)(Arg1, Arg2)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    T* obj = L.template toUserdata<T>(lua_upvalueindex(1));
    RetVal(T::*f)(Arg1, Arg2) = *(L.template toUserdata<RetVal(T::*)(Arg1)>(lua_upvalueindex(2)));
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    Arg2 arg2 = LuaTraits<Arg2>::pop(&L);
    RetVal res = (obj->*f)(arg1, arg2);
    return ReturnValues<RetVal>::apply(&L, res);  
  }
};

};

#endif
