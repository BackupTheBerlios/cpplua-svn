#ifndef LUAPROXYCALL_H
#define LUAPROXYCALL_H
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include "luaiobject.h"

namespace cpplua {

#ifndef CPPLUA_MAX_FUNCTION_ARGS
  #define CPPLUA_MAX_FUNCTION_ARGS 2
#endif

#define CPPLUA_ARG_REF_MACRO(z, n, unused) \
  const BOOST_PP_CAT(Arg,n) & BOOST_PP_CAT(arg,n)
#define CPPLUA_ARG_MEMBERS_MACRO(z, n, unused) \
  const BOOST_PP_CAT(Arg,n) * BOOST_PP_CAT(a,n);
#define CPPLUA_PUSH_ARG_MACRO(z, n, state) \
  LuaTraits<BOOST_PP_CAT(Arg,n)>::push(state, *BOOST_PP_CAT(a,n));
#define CPPLUA_INITIALIZE_ARG_MACRO(z, n, unused) \
  , BOOST_PP_CAT(a,n) ( &BOOST_PP_CAT(arg,n) )
#define CPPLUA_TEMPLATE_ARGS \
  template < BOOST_PP_ENUM_PARAMS(n, typename Arg) >
  
#define BOOST_PP_LOCAL_LIMITS (0, CPPLUA_MAX_FUNCTION_ARGS)
#define BOOST_PP_LOCAL_MACRO(n) \
  BOOST_PP_EXPR_IF(n, CPPLUA_TEMPLATE_ARGS ) \
  class BOOST_PP_CAT(LuaProxyFunctionCall, n) : public LuaIObject { \
    const LuaIObject& f; \
    BOOST_PP_REPEAT(n, CPPLUA_ARG_MEMBERS_MACRO, ~) \
    void push(int nRetVals) const { \
      f.push(); \
      BOOST_PP_REPEAT(n, CPPLUA_PUSH_ARG_MACRO, getState()) \
      LowLevel::protectedCall(getState(), n, nRetVals); \
    } \
  public: \
    BOOST_PP_CAT(LuaProxyFunctionCall,n) (LuaState* L, const LuaIObject& f \
      BOOST_PP_COMMA_IF(n) \
      BOOST_PP_ENUM(n, CPPLUA_ARG_REF_MACRO, ~) \
      ) : LuaIObject(L) \
      , f(f) \
      BOOST_PP_REPEAT(n, CPPLUA_INITIALIZE_ARG_MACRO, ~) \
      {} \
    virtual void push() const { push(1); } \
  };
  
#include BOOST_PP_LOCAL_ITERATE()
#undef CPPLUA_ARG_REF_MACRO
#undef CPPLUA_ARG_MEMBERS_MACRO
#undef CPPLUA_PUSH_ARG_MACRO
#undef CPPLUA_INITIALIZE_ARG_MACRO
#undef CPPLUA_TEMPLATE_ARGS

}; // namespace cpplua

#endif // LUAPROXYCALL_H
