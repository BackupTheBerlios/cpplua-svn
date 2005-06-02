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
#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG

#include "luastate.h"

namespace cpplua {

void LuaState::init() {
  // this table stays at the bottom of lua stack
  newTable();
  insert(1);
}

LuaState::LuaState() {
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
LuaState::LuaState(lua_State* L) : L(L) {
  collectState = false;
  init();
}

/**
 * Destroy all cpplua objects and close the state (if needed).
 */
LuaState::~LuaState() {
  /*  luaPushNil();
  // FIXME
  while (luaNext(cppluaTableIndex)) {
    LuaObject* obj = luaToUserdata<LuaObject*>(-2);
    delete obj;
    luaPop();
    }*/
  
  pop(); // pop cppluaTableIndex

  if (collectState)
    lua_close(L);
}

};
