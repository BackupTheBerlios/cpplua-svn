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

#include <cstring>
#include "common.h"
#include "lowlevel.h"

#ifdef _DEBUG
#define LOG(x) logger << #x << "\n"; x;

class Logger {
private:
  std::ostream& os;
public:
  Logger(std::ostream& os)
    : os(os) {}
  template <typename T>
  Logger& operator<<(const T& obj) {
    os << obj;
    return *this;
  }
};

#else
#define LOG(x) x;
#endif

namespace cpplua {

class LuaIObject;
class LuaObject;
class LuaProxyGlobal;
class LuaProxyEmptyTable;
template <typename T> class LuaProxyPrimitive;
template <typename T, typename Function> class PushMethod;
template <typename Function> class PushFunction;
template <typename T, typename Function> class LuaMethod;
template <typename Function> class LuaFunction;

class LuaState {
  bool collectState;
  lua_State* L;
  void init();
#ifdef _DEBUG
  Logger logger;
  static std::ostream* loggerStream;
#endif  
public:
#ifdef _DEBUG
  static void setLoggerStream(std::ostream* stream) {
    loggerStream = stream;
  }
#endif
  static const int cpptableIndex = 1;
  LuaState();
  LuaState(lua_State*);
  ~LuaState();

  lua_State* getInternalState() { return L; }
  
  LuaProxyGlobal global(const char* name);
  void pushObject(const LuaIObject*);
  LuaProxyEmptyTable emptyTable();
  template <typename T> LuaProxyPrimitive<T> primitive(const T& val);
  // special case for string literals
  template <size_t size>
  LuaProxyPrimitive<const char*> primitive(const char (& val)[size]) {
    return LuaProxyPrimitive<const char*>(this, val);
  }
  template <typename T, typename Function>
  LuaMethod<T, Function> method(const T& obj, Function f) {
    LuaMethod<T, Function> res(this);
    pushLightUserdata(&res);
    PushMethod<T, Function>::apply(this, obj, f);
    setTable(cpptableIndex);
    return res;
  }
  template <typename Function>
  LuaFunction<Function> function(Function f) {
    LuaFunction<Function> res(this);
    pushLightUserdata(&res);
    PushFunction<Function>::apply(this, f);
    setTable(cpptableIndex);
    return res;
  }
  template <typename T, typename Function>
  void registerMethod(const char* name, const T& obj, Function f) {
    global(name) = method(obj, f);
  }
    
  inline int doString(const char* str) {
    loadBuffer(str);
    LowLevelFunctionCall::protectedCall(this, 0, 0);
  }
  
  void printTop() {
    lua_getglobal(L, "print");
    lua_pushvalue(L, -2);
    lua_pcall(L, 1, 0, 0);
  }  

  //BEGIN Wrappers  
    
  // basic libraries
  inline void openBase() {
    LOG(luaopen_base(L));
  }

  inline void openIO() {
    LOG(luaopen_io(L));
  }

  inline void openTable() {
    LOG(luaopen_table(L));
  }

  inline void openString() {
    LOG(luaopen_string(L));
  }

  inline void openMath() {
    LOG(luaopen_math(L));
  }

  inline void pushNil() {
    LOG(lua_pushnil(L));
  }

  inline int getTop() {
    LOG(return lua_gettop(L));
  }

  template <typename Number>
  inline void pushNumber(Number n) {
    lua_pushnumber(L, static_cast<lua_Number>(n));
    logger << "log_pushnumber(L, " << n << ")\n";
  }

  template <typename Number>
  inline Number toNumber(int index = -1) {
    LOG(return static_cast<Number>(lua_tonumber(L, index)));
  }

  template <typename T>
  inline void pushLightUserdata(T* ud) {
    lua_pushlightuserdata(L, static_cast<void*>(ud));
    logger << "log_pushlightuserdata(L, " << ud << ")\n";
  }

  template <typename T>
  inline T* toUserdata(int index = -1) {
    LOG(return static_cast<T*>(lua_touserdata(L, index)));
  }

  inline void pushString(const char* str) {
    lua_pushstring(L, str);
    logger << "lua_pushstring(L, " << str << ")\n";
  }

  inline const char* toString(int index = -1) {
    LOG(return lua_tostring(L, index));
  }

  inline void pushCFunction(lua_CFunction f) {
    LOG(lua_pushcfunction(L, f));
  }
  
  inline void pushCClosure(lua_CFunction f, int upvalues) {
    LOG(lua_pushcclosure(L, f, upvalues));
  }

  inline void newTable() {
    LOG(lua_newtable(L));
  }
  
  inline void* newUserdata(size_t size) {
    LOG(return lua_newuserdata(L, size));
  }

  inline void getTable(int index = -2) {
    lua_gettable(L, index);
    logger << "lua_gettable(L, " << index << ")\n";
  }

  inline void setTable(int index = -3) {
    lua_settable(L, index);
    logger << "lua_settable(L, " << index << ")\n";
  }
  
  inline void getGlobal(const char* name) {
    LOG(lua_pushstring(L, name));
    LOG(lua_gettable(L, LUA_GLOBALSINDEX));
  }

  inline void pop(int count = 1) {
    LOG(lua_pop(L, count));
  }

  inline void pushValue(int index) {
    LOG(lua_pushvalue(L, index));
  }

  inline void insert(int index) {
    lua_insert(L, index);
    logger << "lua_insert(L, " << index << ")\n";
  }

  inline void remove(int index) {
    lua_remove(L, index);
    logger << "lua_remove(L, " << index << ")\n";
  }

  inline bool next(int index = -2) {
    LOG(return (lua_next(L, index) != 0));
  }

  
  inline LuaType type(int index = -1) {
    LOG(return static_cast<LuaType>(lua_type(L, index)));
  }

  inline bool isNumber(int index = -1) {
    LOG(return lua_isnumber(L, index));
  }
  
  inline bool isUserdata(int index = -1) {
    LOG(return lua_isuserdata(L, index));
  }
  
  inline bool isString(int index = -1) {
    LOG(return lua_isstring(L, index));
  }

  inline bool isNil(int index = -1) {
    LOG(return lua_isnil(L, index));
  }

  inline bool isFunction(int index = -1) {
    LOG(return lua_isfunction(L, index));
  }
  
  inline bool isTable(int index = -1) {
    LOG(return lua_istable(L, index));
  }
  
  inline const char* typeName(int index = -1) {
    LOG(return lua_typename(L, lua_type(L, index)));
  }
  
  inline bool equal(int index1 = -2, int index2 = -1) {
    LOG(return lua_equal(L, index1, index2));
  }

  inline void error() {
    LOG(lua_error(L));
  };  
  
  inline int pcall(int nArgs, int nRes, int errFunction) {
    LOG(return lua_pcall(L, nArgs, nRes, errFunction));
  }
  
  inline void loadBuffer(const char* str) {
    LOG(luaL_loadbuffer(L, str, strlen(str), "buffer"));
  }
  
};

// template implementations

template <typename T>
LuaProxyPrimitive<T> LuaState::primitive(const T& val) {
  return LuaProxyPrimitive<T>(this, val);
}

};

#endif
