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
#ifndef FUNCTIONCALL_H
#define FUNCTIONCALL_H

#include "luastate.h"
#include "luaiobject.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cpplua {

//BEGIN FunctionCall
/**
  * Template class to call lua functions.
  * Its static @a apply function pushes a lua
  * function and its arguments on the stack, then
  * calls it.
  * @param RetVal type of the return value.
  * @param n      number of arguments to push
  */
template<typename RetVal, int n>
struct FunctionCall {};

template <typename RetVal>
struct FunctionCall<RetVal, 0> {
  static RetVal apply(LuaState* L, const LuaIObject& f) {
    f.push();
    LowLevelFunctionCall::protectedCall(L, 0, 1);
    return LuaTraits<RetVal>::pop(L);
  }
};

template <typename RetVal>
struct FunctionCall<RetVal, 1> {
  template <typename Arg1>
  static RetVal apply(LuaState* L, const LuaIObject& f, const Arg1& arg1) {
    f.push();
    LuaTraits<Arg1>::push(L, arg1);
    LowLevelFunctionCall::protectedCall(L, 1, 1);
    return LuaTraits<RetVal>::pop(L);
  }
};

template <typename RetVal>
struct FunctionCall<RetVal, 2> {
  template <typename Arg1, typename Arg2>
  static RetVal apply(LuaState* L, const LuaIObject& f, const Arg1& arg1, const Arg2& arg2) {
    f.push();
    LuaTraits<Arg1>::push(L, arg1);
    LuaTraits<Arg2>::push(L, arg2);
    LowLevelFunctionCall::protectedCall(L, 2, 1);
    return LuaTraits<RetVal>::pop(L);
  }
};

template <typename RetVal>
struct FunctionCall<RetVal, 3> {
  template <typename Arg1, typename Arg2, typename Arg3>
  static RetVal apply(LuaState* L, const LuaIObject& f, const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) {
    f.push();
    LuaTraits<Arg1>::push(L, arg1);
    LuaTraits<Arg2>::push(L, arg2);
    LuaTraits<Arg3>::push(L, arg3);
    LowLevelFunctionCall::protectedCall(L, 3, 1);
    return LuaTraits<RetVal>::pop(L);
  }
};
//END FunctionCall

};

#endif
