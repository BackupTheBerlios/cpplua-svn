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

#ifndef LUAOBJECT_H
#define LUAOBJECT_H

#include "luaiobject.h"
#include "luabracket.h"
#include "functioncall.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {

class LuaState;

class LuaObject : public LuaIObject {
public:
  explicit LuaObject(LuaState* L);
  LuaObject(const LuaIObject&);
  LuaObject(const LuaObject&);
  
  template <typename T>
  LuaObject& operator=(const T& obj) {
    duplicate(obj);
    return *this;
  }
  // for some reasons I'd like to understand
  // the compiler doesn't call the template
  // assignment operator when T = LuaObject
  LuaObject& operator=(const LuaObject& obj) {
    duplicate(obj);
    return *this;
  }
  
  
  void push() const;
  
  template <typename T>
  LuaBracket<LuaObject, T> operator[](const T& index) const {
    return LuaBracket<LuaObject, T>(getState(), *this, index);
  }
  
};

};

#endif // LUAOBJECT_H
