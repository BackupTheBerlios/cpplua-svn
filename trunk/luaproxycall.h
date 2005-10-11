#ifndef LUAPROXYCALL_H
#define LUAPROXYCALL_H

#include <boost/tuple/tuple.hpp>

#include "luaiobject.h"
#include "utils/tuples.h"

namespace cpplua {

using namespace boost;

template <typename ArgTuple>
class LuaProxyFunctionCall : public LuaIObject {
  const LuaIObject& function;
  const ArgTuple& args;
  void push(int nRetVals) const {
    function.push();
    Map<LuaTraitsPush>::apply(getState(), args);
    LowLevel::protectedCall(getState(), tuples::length<ArgTuple>::value, nRetVals);
  }
public:
  LuaProxyFunctionCall(LuaState* L, const LuaIObject& function, const ArgTuple& args)
  : LuaIObject(L)
  , function(function)
  , args(args) {}
  virtual void push() const { push(1); }
};

/**
  * Convenience class for 0 argument calls.
  */
class LuaProxyFunctionCall0 : public LuaProxyFunctionCall< tuple<> > {
public:
  LuaProxyFunctionCall0(LuaState* L, const LuaIObject& function)
  : LuaProxyFunctionCall< tuple<> >(L, function, tuple<>()) {}
};


}; // namespace cpplua

#endif // LUAPROXYCALL_H
