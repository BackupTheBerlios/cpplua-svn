#include "lowlevel.h"
#include "luastate.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {

void LowLevel::protectedCall(LuaState* L, int nArgs, int nRetVals) {
  // No need for an error handler function.
  // The stack needs to be unwound before
  // any C++ error handling mechanism can
  // take place.
  if(L->pcall(nArgs, nRetVals, 0)) {
    // FIXME: Is it possible to find out
    // precisely if the error was caused by cpplua?

    if (L->isUserdata()) {
      // retrieve exception from the stack
      cpplua_error* exc_pointer = L->toUserdata<cpplua_error>();
      L->pop();
      
      // copy it
      cpplua_error exception = *exc_pointer;
      delete exc_pointer;
      
      // and finally, throw it!
      throw exception;
    }
    else
      handleError(L);
  }
}

void LowLevel::handleError(LuaState* L) {
  if (L->isString()) {
    // Throw a generic cpplua exception.
    cpplua_error exception(L->toString());
    L->pop();
    throw exception;
  }
  else {
    cpplua_error exception("unknown error");
    L->pop();
    throw exception;
  }
}

};
