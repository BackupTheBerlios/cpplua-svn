#include "method.h"

class A {
  int x;
public:
  A(int x) : x(x) {}
  int square() {
    return x*x;
  }
  
  int times(int y) {
    return x * y;
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(MethodTest);

void MethodTest::setUp() {
  L = new LuaState();
}

void MethodTest::tearDown() {
  delete L;
}

void MethodTest::method() {
  A a(6);
  
  LuaMethod<A, int(A::*)()> square6 = L->method(a, &A::square);
  CPPUNIT_ASSERT(square6() == 36);
  L->registerMethod("sixtimes", a, &A::times);
  L->doString("temp = sixtimes(5)");
  CPPUNIT_ASSERT(L->global("temp") == 30);
}
