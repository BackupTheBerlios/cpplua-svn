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

int test() {
  return 37;
}

int sum(int a, int b, const char* msg) {
  int res = a + b;
  cout << msg << " " << res << endl;
  return res;
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
    
    (*L)
      .reg("test", test)
    ;
    
    L->doFile("test.lua");
  }
  
  delete stream;
  return 0;
}

#undef LG
