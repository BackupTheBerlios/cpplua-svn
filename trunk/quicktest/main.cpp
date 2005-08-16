#include <iostream>
#include <cassert>
#include "../cpplua.h"

using namespace std;
using namespace cpplua;

int main() {
  LuaState L;
  
  L.doString("x = {}");
  LuaObject x = L.global("x");
  x[5] = "hello world";
  L.doString("print(x[5])");

}
