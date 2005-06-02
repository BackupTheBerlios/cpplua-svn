/***************************************************************************
 *   Copyright (C) 2004 by Paolo Capriotti                                 *
 *   p.capriotti@sns.it                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef LUASTATE_H
#define LUASTATE_H

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif // _DEBUG

#include "common.h"

namespace cpplua {

class LuaState {
  bool collectState;
  lua_State* L;
  LuaState(lua_State*);
  void init();
  
public:
  static const int cpptableIndex = 1;
  LuaState();
  ~LuaState();

  // debug
  lua_State* getInternalState() { return L; }
  
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

  inline void newTable() {
    lua_newtable(L);
  }

  inline void getTable(int index = -2) {
    lua_gettable(L, index);
  }

  inline void setTable(int index = -3) {
    lua_settable(L, index);
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

};

}

#endif
