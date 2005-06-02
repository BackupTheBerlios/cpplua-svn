#ifndef COMMON_H
#define COMMON_H

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

enum LuaType {
  NilType = LUA_TNIL,
  NumberType = LUA_TNUMBER, 
  BooleanType = LUA_TBOOLEAN, 
  StringType = LUA_TSTRING,
  TableType = LUA_TTABLE, 
  FunctionType = LUA_TFUNCTION, 
  UserDataType = LUA_TUSERDATA,
  ThreadType = LUA_TTHREAD, 
  LightUserDataType = LUA_TLIGHTUSERDATA,
};
  


#endif // COMMON_H
