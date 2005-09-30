#ifndef LOWLEVEL_H
#define LOWLEVEL_H

// BEGIN LowLevelFunctionCall

namespace cpplua {

class LuaState;

/**
  * Handle low level (lua-related) details of
  * function calling and error handling.
  */
class LowLevelFunctionCall {
public:
  static void protectedCall(LuaState*, int, int);
};
// END

}

#endif
