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

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {

class LuaState;

/**
  * @brief A reference to a Lua object
  * 
  * A LuaObject is the basic building block of CppLua. 
  * Every Lua object can be held by reference in a C++ 
  * variable of type LuaObject. 
  * CppLua keeps track of the various LuaObjects using
  * a global table at the bottom of the LuaStack.
  */
class LuaObject : public LuaIObject {
public:
  explicit LuaObject(LuaState* L);
  LuaObject(const LuaIObject&);
  LuaObject(const LuaObject&);

  /**
    * @brief Assignment with reference semantics.
    */
  template <typename T>
  LuaObject& operator=(const T& obj) {
    duplicate(obj);
    return *this;
  }

  /**
    * @brief Assignment with reference semantics.
    */  
  LuaObject& operator=(const LuaObject& obj) {
    duplicate(obj);
    return *this;
  }

  /**
    * @brief Push the object on the Lua stack.
    */
  void push() const;
  
  template <typename T>
  LuaBracket<LuaObject, T> operator[](const T& key) const {
    return LuaBracket<LuaObject, T>(getState(), *this, key);
  }  
};

};

#endif // LUAOBJECT_H
