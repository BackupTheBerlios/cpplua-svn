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

#ifndef LUASTATE_H
#define LUASTATE_H

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif // _DEBUG

#include "common.h"

namespace cpplua {

class LuaIObject;
class LuaObject;

class LuaState {
  bool collectState;
  lua_State* L;
  LuaState(lua_State*);
  void init();
  
public:
  static const int cpptableIndex = 1;
  LuaState();
  ~LuaState();

  lua_State* getInternalState() { return L; }

  void duplicateObject(const LuaIObject* src, const LuaIObject* dest);
  LuaObject global(const char* name);
  void pushObject(const LuaIObject*);
  
  // basic libraries
  inline void openBase() {
    luaopen_base(L);
  }

  inline void openIO() {
    luaopen_io(L);
  }

  inline void openTable() {
    luaopen_table(L);
  }

  inline void openString() {
    luaopen_string(L);
  }

  inline void openMath() {
    luaopen_math(L);
  }

  // wrappers
  inline void pushNil() {
    lua_pushnil(L);
  }

  inline int getTop() {
    return lua_gettop(L);
  }

  template <typename Number>
  inline void pushNumber(Number n) {
    lua_pushnumber(L, static_cast<lua_Number>(n));
  }

  template <typename Number>
  inline Number toNumber(int index = -1) {
    return static_cast<Number>(lua_tonumber(L, index));
  }

  template <typename T>
  inline void pushLightUserdata(T* ud) {
    lua_pushlightuserdata(L, static_cast<T*>(ud));
  }

  template <typename T>
  inline T* toUserdata(int index = -1) {
    return static_cast<T*>(lua_touserdata(L, index));
  }

  inline void pushString(const char* str) {
    lua_pushstring(L, str);
  }

  inline const char* toString(int index = -1) {
    return lua_tostring(L, index);
  }

  inline void pushCFunction(lua_CFunction f) {
    lua_pushcfunction(L, f);
  }

  inline void newTable() {
    lua_newtable(L);
  }

  inline void getTable(int index = -2) {
    lua_gettable(L, index);
  }

  inline void setTable(int index = -3) {
    lua_settable(L, index);
  }
  
  inline void getGlobal(const char* name) {
    lua_pushstring(L, name);
    lua_gettable(L, LUA_GLOBALSINDEX);
  }

  inline void pop(int count = 1) {
    lua_pop(L, count);
  }

  inline void pushValue(int index) {
    lua_pushvalue(L, index);
  }

  inline void insert(int index) {
    lua_insert(L, index);
  }

  inline void remove(int index) {
    lua_remove(L, index);
  }

  inline bool next(int index = -2) {
    return (lua_next(L, index) != 0);
  }

  
  inline LuaType type(int index = -1) {
    return static_cast<LuaType>(lua_type(L, index));
  }

  inline bool isNumber(int index = -1) {
    return lua_isnumber(L, index);
  }
  
  inline bool isUserdata(int index = -1) {
    return lua_isuserdata(L, index);
  }
  
  inline bool isString(int index = -1) {
    return lua_isstring(L, index);
  }

  inline bool isNil(int index = -1) {
    return lua_isnil(L, index);
  }

  inline bool isFunction(int index = -1) {
    return lua_isfunction(L, index);
  }
  
  inline bool isTable(int index = -1) {
    return lua_istable(L, index);
  }
  
  inline const char* typeName(int index = -1) {
    return lua_typename(L, lua_type(L, index));
  }
  
  inline bool equal(int index1 = -2, int index2 = -1) {
    return lua_equal(L, index1, index2);
  }
  

  inline int pcall(int nArgs, int nRes, int errFunction) {
    return lua_pcall(L, nArgs, nRes, errFunction);
  }
  
  inline int doString(const char* str) {
    return lua_dostring(L, str);
  }
  
};

}

#endif
