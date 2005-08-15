#include <iostream>
#include <cassert>
#include "../cpplua.h"

using namespace std;
using namespace cpplua;

int main() {
  LuaState* L = new LuaState();
  
  L->doString("t = {} t[7] = 12");
  
  cout << "before LuaObject t = L->global(\"t\");" << endl;
  LuaObject t = L->global("t");
  cout << "after LuaObject t = L->global(\"t\");" << endl;
  
  L->doString("t = nil");
  
  cout << "before t[7].typeName()" << endl;
  cout << t[7].typeName() << endl;
  cout << "after t[7].typeName()" << endl;
  cout << t[7].type() << endl;
  cout << t[7].toNumber<int>() << endl;
}
