#ifndef FUNCTION_FACTORY_H
#define FUNCTION_FACTORY_H

#include <vector>
#include "luatraits.h"
#include "luastate.h"
#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif


namespace cpplua {

typedef std::vector<LuaObject> LuaCollection;
template <typename T, typename Function> struct GeneralMethod;
template <typename Function> struct GeneralFunction;

// metafunctions

template <typename Arg>
struct RetrieveFirstArgument {
  static Arg apply(LuaState* L) {
    return LuaTraits<Arg>::pop(L);
  }
};

// template <>
// struct RetrieveFirstArgument<LuaState*> {
//   static LuaState* apply(LuaState* L) {
//     return L;
//   }
// };


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



// Push Method

template <typename T, typename Function>
struct PushMethod {
  static void apply(LuaState* L, const T& obj, Function f);
};

template <typename T, typename Function>
void PushMethod<T, Function>::apply(LuaState* L, const T& obj, Function f) {
  L->pushLightUserdata(const_cast<T*>(&obj));
  Function* ptmf = reinterpret_cast<Function*>(
    L->newUserdata(sizeof(Function))
  );
  *ptmf = f;
  L->pushCClosure(GeneralMethod<T, Function>::apply, 2);
}

// Push Function

template <typename Function>
struct PushFunction {
  static void apply(LuaState* L, Function f);
};

template <typename Function>
void PushFunction<Function>::apply(LuaState* L, Function f) {
  Function* ptmf = reinterpret_cast<Function*>(
    L->newUserdata(sizeof(Function))
  );
  *ptmf = f;
  L->pushCClosure(GeneralFunction<Function>::apply, 1);
}

// General Method

template <typename T, typename Function> struct GeneralMethod {};

template <typename T, typename Function>
struct GeneralMethod<T, Function const> 
  : public GeneralMethod<T, Function> {};

template <typename RetVal, typename T>
struct GeneralMethod<T, RetVal(T::*)()> {
  static int apply(lua_State* l) {
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

// General Function

template <typename Function> struct GeneralFunction {};

template <typename RetVal>
struct GeneralFunction<RetVal(*)()> {
  static int apply(lua_State* l) {
    LuaState L(l);
    RetVal(*f)() = *(L.template toUserdata<RetVal(*)()>(lua_upvalueindex(1)));
    RetVal res = f();
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

template <typename RetVal, typename Arg1>
struct GeneralFunction<RetVal(*)(Arg1)> {
  static int apply(lua_State* l) {
    cerr << "inside apply with 1 argument" << endl;
    LuaState L(l);
    RetVal(*f)(Arg1) = *(L.template toUserdata<RetVal(*)(Arg1)>(lua_upvalueindex(1)));
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    cerr << "first argument = " << arg1 << endl;
    RetVal res = f(arg1);
    cerr << "retval = " << res << endl;
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

template <typename RetVal, typename Arg1, typename Arg2>
struct GeneralFunction<RetVal(*)(Arg1, Arg2)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    RetVal(*f)(Arg1, Arg2) = *(L.template toUserdata<RetVal(*)(Arg1, Arg2)>(lua_upvalueindex(1)));
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    Arg2 arg2 = LuaTraits<Arg2>::pop(&L);    
    RetVal res = f(arg1, arg2);
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

template <typename RetVal, typename Arg1, typename Arg2, typename Arg3>
struct GeneralFunction<RetVal(*)(Arg1, Arg2, Arg3)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    RetVal(*f)(Arg1, Arg2, Arg3) = *(L.template toUserdata<RetVal(*)(Arg1,Arg2,Arg3)>(lua_upvalueindex(1)));
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    Arg2 arg2 = LuaTraits<Arg2>::pop(&L);
    Arg3 arg3 = LuaTraits<Arg3>::pop(&L);
    RetVal res = f(arg1, arg2, arg3);
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

/* if C++ had variable argument list

template <typename RetVal, multiple typename Args>
struct GeneralFunction<RetVal(*)(multiple Args)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    multiple Args args;
    RetVal(*f)(Args) = *(L.template toUserdata<RetVal(*)(multiple Args)>(lua_upvalueindex(1)));
    if (sizeof(Args) > 0) {
      args[1] = RetrieveFirstArgument<Args[1]>::apply(&L);
      for(int i = 2; i < sizeof(Args); i++)
        args[i] = LuaTraits<Args[i]>::pop(&L);
    }
    RetVal res = f(multiple args);
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

*/

};

#endif
