#ifndef FUNCTION_FACTORY_H
#define FUNCTION_FACTORY_H

#include <vector>
#include <sstream>
#include "luatraits.h"
#include "luastate.h"
#include "utils/traits.h"
#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif


namespace cpplua {

typedef std::vector<LuaObject> LuaCollection;
template <typename T, typename Function> struct GeneralMethod;
template <typename Function> struct GeneralFunction;

//BEGIN Validation

class ArgumentNumberError : public cpplua_error {
private:
  static std::string msg(int expected, int given) {
    std::ostringstream os;
    os << "wrong number of arguments"
       << "\n\texpected: " << expected
       << "\n\tgiven:    " << given;
    return os.str();
  }
public:
  ArgumentNumberError(int expected, int given)
  : cpplua_error(msg(expected, given))
  {}
};

//END Validation

//BEGIN Metafunctions

template <typename Arg>
struct RetrieveFirstArgument {
  static const int argCount = 1;
  static Arg apply(LuaState* L) {
    return LuaTraits<Arg>::pop(L);
  }
};

template <>
struct RetrieveFirstArgument<LuaState*> {
  static const int argCount = 0;
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

template <typename ArgTuple>
struct RetrieveArguments {
  typedef typename ArgTuple::head_type Arg0;
  typedef typename ArgTuple::tail_type OtherArgs;

  static const int argCount = RetrieveFirstArgument<Arg0>::argCount +
                              tuples::length<OtherArgs>::value;
  static ArgTuple apply(LuaState* L) {
    OtherArgs otherArgs = Map<LuaTraitsPop>::template revTransform<LuaState*, OtherArgs>(L);
    Arg0 arg0 = RetrieveFirstArgument<Arg0>::apply(L);
    return tuples::cons<Arg0, OtherArgs>(arg0, otherArgs);
  };
};

template <>
struct RetrieveArguments< tuple<> > {
  static const int argCount = 0;
  static tuple<> apply(LuaState* ) {
    // do nothing
    return make_tuple();
  }
};

//END Metafunctions

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
    RetVal(T::*f)(Arg1, Arg2) = *(L.template toUserdata<RetVal(T::*)(Arg1, Arg2)>(lua_upvalueindex(2)));
    Arg2 arg2 = LuaTraits<Arg2>::pop(&L);
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    RetVal res = (obj->*f)(arg1, arg2);
    return ReturnValues<RetVal>::apply(&L, res);  
  }
};

template <typename RetVal, typename T, typename Arg1, typename Arg2, typename Arg3>
struct GeneralMethod<T, RetVal(T::*)(Arg1, Arg2, Arg3)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    T* obj = L.template toUserdata<T>(lua_upvalueindex(1));
    RetVal(T::*f)(Arg1, Arg2) = *(L.template toUserdata<RetVal(T::*)(Arg1, Arg2, Arg3)>(lua_upvalueindex(2)));
    Arg3 arg3 = LuaTraits<Arg3>::pop(&L);
    Arg2 arg2 = LuaTraits<Arg2>::pop(&L);
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    RetVal res = (obj->*f)(arg1, arg2, arg3);
    return ReturnValues<RetVal>::apply(&L, res);  
  }
};

// General Function

template <typename Function> struct GeneralFunction {
  typedef FunctionTraits< typename remove_pointer <Function>::type > f_traits;
  typedef typename f_traits::result_type RetVal;
  typedef typename f_traits::args_type ArgTuple;
  
  static int apply(lua_State* l) {
    LuaState L(l);
    
    // validation
    const int n = RetrieveArguments<ArgTuple>::argCount;
    {
      const int nArgs = L.getTop() - 1;
      if (nArgs != n) {
        L.pushString(ArgumentNumberError(n, nArgs).what());
        L.error();
      }
    }    
    
    Function f = *(L.template toUserdata<Function>(lua_upvalueindex(1)));
    ArgTuple args = RetrieveArguments<ArgTuple>::apply(&L);
    RetVal res = ApplyFunctionToTuple<Function, ArgTuple>::apply(f, args);
    return ReturnValues<RetVal>::apply(&L, res);
  }
};
/*
template <typename Function> struct GeneralFunction {};

#define VALIDATE(N) \
{ \
  int n = (N); \
  int nArgs = L.getTop() - 1; \
  if (nArgs != n) { \
    L.pushString(ArgumentNumberError(n, nArgs).what()); \
    L.error(); \
  } \
}

template <typename RetVal>
struct GeneralFunction<RetVal(*)()> {
  static int apply(lua_State* l) {
    LuaState L(l);
    VALIDATE(0);
    RetVal(*f)() = *(L.template toUserdata<RetVal(*)()>(lua_upvalueindex(1)));
    RetVal res = f();
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

template <typename RetVal, typename Arg1>
struct GeneralFunction<RetVal(*)(Arg1)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    VALIDATE(RetrieveFirstArgument<Arg1>::argCount);
    RetVal(*f)(Arg1) = *(L.template toUserdata<RetVal(*)(Arg1)>(lua_upvalueindex(1)));
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    RetVal res = f(arg1);
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

template <typename RetVal, typename Arg1, typename Arg2>
struct GeneralFunction<RetVal(*)(Arg1, Arg2)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    VALIDATE(1 + RetrieveFirstArgument<Arg1>::argCount);
    RetVal(*f)(Arg1, Arg2) = *(L.template toUserdata<RetVal(*)(Arg1, Arg2)>(lua_upvalueindex(1)));
    Arg2 arg2 = LuaTraits<Arg2>::pop(&L);
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    RetVal res = f(arg1, arg2);
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

template <typename RetVal, typename Arg1, typename Arg2, typename Arg3>
struct GeneralFunction<RetVal(*)(Arg1, Arg2, Arg3)> {
  static int apply(lua_State* l) {
    LuaState L(l);
    VALIDATE(2 + RetrieveFirstArgument<Arg1>::argCount);
    RetVal(*f)(Arg1, Arg2, Arg3) = *(L.template toUserdata<RetVal(*)(Arg1,Arg2,Arg3)>(lua_upvalueindex(1)));
    Arg3 arg3 = LuaTraits<Arg3>::pop(&L);
    Arg2 arg2 = LuaTraits<Arg2>::pop(&L);        
    Arg1 arg1 = RetrieveFirstArgument<Arg1>::apply(&L);
    RetVal res = f(arg1, arg2, arg3);
    return ReturnValues<RetVal>::apply(&L, res);
  }
};

#undef VALIDATE*/

};

#endif
