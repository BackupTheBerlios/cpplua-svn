#ifndef LOWLEVEL_H
#define LOWLEVEL_H

//BEGIN LowLevelFunctionCall

#include "common.h"

namespace cpplua {

class LuaState;

/**
  * Handle low level (lua-related) details of
  * function calling and error handling.
  */
class LowLevel {
public:
  /**
    * Call a lua function, setting @a errorHandler
    * as the error handler.
    */
  static void protectedCall(LuaState*, int, int);
  static int errorHandler(lua_State*);
  static void handleError(LuaState*);
};
//END

}

#endif
