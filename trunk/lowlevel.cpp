#include "lowlevel.h"
#include "luastate.h"

namespace cpplua {

void LowLevelFunctionCall::protectedCall(LuaState* L, int nArgs, int nRetVals) {
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
    else {
      // Assume the error is a string.
      // Throw a generic cpplua exception.
      cpplua_error exception(L->toString());
      L->pop();
      throw exception;
    }
  }
}

};