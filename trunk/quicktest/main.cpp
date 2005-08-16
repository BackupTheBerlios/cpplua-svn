#include <iostream>
#include <cassert>
#include "../cpplua.h"

using namespace std;
using namespace cpplua;

int main() {
  LuaState L;
  
  L.doString("x = 2");
  LuaObject x = L.global("x");
  
  L.doString("x = 4");
  cout << "before assignment" << endl;
  x = L.global("x");
  cout << "after assignment" << endl;
  
  cout << x.toNumber<int>() << endl;
}
