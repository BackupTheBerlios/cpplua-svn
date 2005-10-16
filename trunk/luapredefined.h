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

#ifndef LUAPREDEFINED_H
#define LUAPREDEFINED_H

#include "luaobject.h"

#define CPPLUA_LUATABLE_OPERATOR_EQUALS() \


namespace cpplua {

/**
  * @brief A specialized LuaObject for lua tables.
  *
  * A @code LuaTable @endcode is a LuaObject which can only hold lua tables.
  * If used as an argument type for a function registered into lua, CppLua will
  * enforce typechecking.
  */
class LuaTable : public LuaObject {
  LuaIObject* typecheck(const LuaIObject& obj);
public:
  explicit LuaTable(LuaState* L);
  LuaTable(const LuaTable&);
  LuaTable(const LuaIObject&); // can throw
  
  template <typename T>
  LuaTable& operator=(const T& obj) {
    if (LuaTraits<T>::type(obj) != TableType)
      throw cpplua_error("Type mismatch in table assignment");
    LuaObject::operator=(obj);
    return *this;
  }
  
  LuaTable& operator=(const LuaTable& other) {
    LuaObject::operator=(other);
    return *this;
  }
};

}; // namespace cpplua

#undef CPPLUA_LUATABLE_OPERATOR_EQUALS

#endif // LUAPREDEFINED_H
