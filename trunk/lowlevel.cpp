#include "lowlevel.h"
#include "luastate.h"
#include <memory>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {

void LowLevel::protectedCall(LuaState* L, int nArgs, int nRetVals) {
  L->pushCFunction(errorHandler);
  int errorHandlerIndex = L->getTop() -nArgs - 1;
  L->insert(errorHandlerIndex);
    
  if(L->pcall(nArgs, nRetVals, errorHandlerIndex)) {
    // retrieve exception from the stack
    cpplua_error* exc_pointer = L->toUserdata<cpplua_error>();
    L->pop();
    
    // copy it
    cpplua_error exception = *exc_pointer;
    delete exc_pointer;
        
    // and finally, throw it!
    throw exception;
  }

  // remove error handler function
  L->remove(errorHandlerIndex);
}

int LowLevel::errorHandler(lua_State* l) {
  auto_ptr<LuaState> L(new LuaState(l));

  cpplua_error* exception = new cpplua_error(L->toString());
  L->pop();
  L->pushLightUserdata(exception);
  return 1;
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
