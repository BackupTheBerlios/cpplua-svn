#include <iostream>
#include <fstream>
#include <cassert>
#include <memory>
#include "../cpplua.h"

using namespace std;
using namespace cpplua;

class A {
  int a;
public:
  A(int a) : a(a) {}
  int sum(int x) const {
    return x + a;
  }
};

int sub(int a, int b) {
  return a - b;
}

int square(int x) {
  return x * x;
}

double dSquare(double x) {
  return x * x;
}

LuaObject identityIndex(LuaObject table, LuaObject key) {
  return key;
}

#define LG(x) L->global(#x)

int main(int argc, char** argv) {
  ofstream* stream = 0;
  if (argc > 1) {
    stream = new ofstream(argv[1]);
    LuaState::setLoggerStream(stream);
  }

  {
    auto_ptr<LuaState> L(new LuaState);  
    
    LuaTuple<1> t(L->global("x"));
    
  }
  
  delete stream;
  return 0;
}

#undef LG
