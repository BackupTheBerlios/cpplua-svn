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

#ifndef LUAIOBJECT_H
#define LUAIOBJECT_H

#include "luastate.h"
#include "luatraits.h"

namespace cpplua {

/**
 * LuaIObject is a common interface for all Lua objects handled by cpplua.
 */
class LuaIObject {
  LuaState& L;
protected:
  inline LuaState& getState() const {
    return L;
  }
public:
  explicit LuaIObject(LuaState& L) : L(L) {}
  virtual void push() const = 0;
  
  /**
  * Default implementation for equality
  */
  template <typename T>
  bool operator==(const T& obj) const {
    push();
    LuaTraits<T>::push(&getState(), obj);
    
    bool res = getState().equal();    
        
    getState().pop();
    getState().pop();
    
    return res;
  }
  
  bool isFunction() const;
  bool isNumber() const;
  bool isString() const;
  bool isNil() const;
  bool isUserdata() const;
  
  LuaType type() const;
  const char* typeName() const;
  
  template <typename T>
  T toNumber() const {
    push();
    T res = getState().toNumber<T>();
    getState().pop();
    return res;
  }
};


};
#endif
