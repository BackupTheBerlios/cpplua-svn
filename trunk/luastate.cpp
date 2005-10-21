/*
Copyright (c) 2005 Paolo Capriotti

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifdef _DEBUG
#include <cassert>
#include <iostream>
using namespace std;
#endif // _DEBUG

#include "luastate.h"
#include "luaobject.h"
#include "luaproxy.h"

namespace cpplua {

#ifdef _DEBUG
std::ostream* LuaStateWrapper::loggerStream = &std::cerr;
#endif

void LuaState::init() {
  // this table stays at the bottom of lua stack
  newTable();
  insert(1);
}

LuaState::LuaState() 
{
  #ifdef _DEBUG
    logger << " ---\n";
    logger << "new LuaState at " << this << "\n";
  #endif
  // states created with the default constructor
  // must be closed at destruction time.
  L = lua_open();
  collectState = true;
  
  openBase();
  init();
}

/**
 * This constructor is only used internally by cpplua
 * to provide a wrapper to a lua_State.
 */
LuaState::LuaState(lua_State* L) 
  : LuaStateWrapper(L) 
{
  #ifdef _DEBUG
    logger << " ---\n";
    logger << "new LuaState at " << this << " from lua_State " << L << "\n";
  #endif
  collectState = false;
  init();
}

/**
 * Destroy all cpplua objects and close the state (if needed).
 */
LuaState::~LuaState() {
// FIXME
//   luaPushNil();
//   while (luaNext(cppluaTableIndex)) {
//     LuaObject* obj = luaToUserdata<LuaObject*>(-2);
//     delete obj;
//     luaPop();
//   }
  
   remove(cpptableIndex);

  if (collectState)
    lua_close(L);
  
#ifdef _DEBUG
  logger << "LuaState " << this << " destroyed\n\n";
#endif
}

/**
  * Retrieve a global object
  */
LuaProxyGlobal LuaState::global(const char* name) {
  return LuaProxyGlobal(this, name);
}

LuaProxyGlobal LuaState::operator[](const char* name) {
  return global(name);
}

LuaProxyEmptyTable LuaState::emptyTable() {
  return LuaProxyEmptyTable(this);
}

/**
  * Push a registered LuaIObject on the stack
  */
void LuaState::pushObject(const LuaIObject* obj) {
  pushLightUserdata(const_cast<LuaIObject*>(obj));
  getTable(cpptableIndex);
}



};
