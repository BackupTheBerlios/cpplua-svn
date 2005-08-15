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

#ifndef LUABRACKET_H
#define LUABRACKET_H

#include "luaiobject.h"
#include "luatraits.h"

namespace cpplua {

template <typename Table, typename Key>
class LuaBracket : public LuaIObject {
  const Table& table;
  const Key& key;
public:
  explicit LuaBracket(LuaState* L, const Table& table, const Key& key) : 
    LuaIObject(*L),
    table(table),
    key(key)
  {}

  void push() const {
    LuaTraits<Table>::push(&getState(), table);
    LuaTraits<Key>::push(&getState(), key);
    getState().getTable();
    getState().remove(-2);
  }
  
  
  template <typename T>
  LuaBracket<Table, Key> operator=(const T& obj) {
    LuaTraits<Table>::push(&getState(), table);
    LuaTraits<Key>::push(&getState(), key);
    LuaTraits<T>::push(&getState(), obj);
    getState().setTable();
    getState().pop();
    
    return *this;
  }
   
};

};

#endif
