#include "function.h"

double square(double x) {
  return x*x;
}

int setGlobal2(LuaState* L, const char* name, LuaObject obj) {
  L->pushString(name);
  obj.push();
  L->setTable(LUA_GLOBALSINDEX);
  return 0;
}

CPPUNIT_TEST_SUITE_REGISTRATION(FunctionTest);

void FunctionTest::setUp() {
  L = new LuaState();
}

void FunctionTest::tearDown() {
  delete L;
}

void FunctionTest::globalFunction() {
  LuaFunction<double(*)(double)> f = L->function(square);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.51*3.51,f(3.51), 1e-10);
  
  LuaFunction<int(*)(LuaState*, const char*, LuaObject)> g = L->function(setGlobal2);
  g("hello",L->primitive("world"));
//  CPPUNIT_ASSERT(L->global("hello") == "world"); // FIXME
}

void FunctionTest::luaObjects() {
  L->global("f") = L->function(square);
  LuaObject f = L->global("f");
  double res = f(21.72).toNumber<double>();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(21.72*21.72, res, 1e-10);
}
