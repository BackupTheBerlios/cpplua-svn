#ifndef LUATRAITS_H
#define LUATRAITS_H

#include <boost/type_traits.hpp>
#include <boost/mpl/if.hpp>
#include "luaiobject.h"
#include "luastate.h"

namespace cpplua {

template <typename T>
class validation_error : public cpplua_error {
public:
  validation_error()
  : cpplua_error("validation error") {}
};

/** 
  * @brief Generic LuaTraits
  * 
  * Assume a LuaIObject-like interface.
  * 
  */
template <typename T>
struct LuaTraits {
  static void push(LuaState*, const T& obj) {
    obj.push();
  }
  
  static T pop(LuaState* L) {
    T res(L);
    L->pushLightUserdata(&res);
    L->pushValue(-2);
    L->setTable(LuaState::cpptableIndex);
    L->pop();
    return res;
  }
  
  static LuaType type(const T& obj) {
    return obj.type();
  }

  static bool validate(LuaState*) {
    return true;
  }
};

// userdata specialization
// FIXME: this should not be necessary
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
  static LuaType type(T* const &) {
    return UserDataType;
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
  static LuaType type(const int&) {
    return NumberType;
  }
  static bool validate(LuaState* L) {
    return L->isNumber();
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
  static LuaType type(const int&) {
    return NumberType;
  }
  static bool validate(LuaState* L) {
    return L->isNumber();
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
  static LuaType type(const int&) {
    return StringType;
  }
  static bool validate(LuaState* L) {
    return L->isString();
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
  static LuaType type(const int&) {
    return StringType;
  }
  static bool validate(LuaState* L) {
    return L->isString();
  }
};

// BEGIN LuaTraits metafunction interface

struct LuaTraitsPush {
  template <typename T>
  struct apply {
    typedef typename boost::remove_const<
      typename boost::remove_reference<T>::type
    >::type RealT;  
    static void value(LuaState* L, const RealT& x) {
      LuaTraits<RealT>::push(L, x);
    }
  };
};

struct LuaTraitsPop {
  template <typename T>
  struct apply {
    static T value(LuaState* L) {
      return LuaTraits<T>::pop(L);
    }
  };
};

struct LuaValidateAndPop {
  template <typename T>
  struct apply {
    static T value(LuaState* L) {
      if (!LuaTraits<T>::validate(L))
	throw validation_error<T>();
      return LuaTraits<T>::pop(L);
    }
  };
};

// END LuaTraits metafunction interface

};

#endif
