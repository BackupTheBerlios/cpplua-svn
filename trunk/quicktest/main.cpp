#include <iostream>
#include <fstream>
#include <cassert>
#include "../cpplua.h"

using namespace std;
using namespace cpplua;

class A {
  int a;
public:
  A(int a) : a(a) {}
  int sum(int x) {
    return x + a;
  }
};

int square(int x) {
  return x * x;
}

int main(int argc, char** argv) {
  ofstream* stream = 0;
  if (argc > 1) {
    stream = new ofstream(argv[1]);
    LuaState::setLoggerStream(stream);
  }

  {
    LuaState L;
    A a(9);
      
    L.global("f") = L.method(a, &A::sum);
    L.doString("temp = f(5)");
    assert(L.global("temp") == 14);
  }
  
  delete stream;
  return 0;
}
