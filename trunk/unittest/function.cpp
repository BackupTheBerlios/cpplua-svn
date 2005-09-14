#include "function.h"

#include <iostream>
using namespace std;

double square(double x) {
  return x*x;
}

const char* constant() {
  return "all you base are belong to us";
}

int sum(int x, int y) {
  return x+(y*2);
}

int setGlobal2(LuaState* L, const char* name, LuaObject obj) {
  L->pushString(name);
  obj.push();
  L->setTable(LUA_GLOBALSINDEX);
  
  return 0;
}

// create a table t and set t[key] = value;
LuaObject nt(LuaState* L, LuaObject key, LuaObject value) {
  LuaObject table = L->emptyTable();
  table[key] = value;
  return table;
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
  CPPUNIT_ASSERT_DOUBLES_EQUAL(3.51*3.51,f(3.51), 1e-5);
  
  LuaFunction<int(*)(LuaState*, const char*, LuaObject)> g = L->function(setGlobal2);
  g("hello",L->primitive("world"));
  CPPUNIT_ASSERT(L->global("hello") == "world");
}

void FunctionTest::luaObjects() {
  // test 0 argument function
  // via LuaProxyGlobal
  L->global("f0") = L->function(constant);
  CPPUNIT_ASSERT(L->global("f0")() == constant());

  // test 1 argument function
  // via LuaObject
  L->global("f1") = L->function(square);
  LuaObject f = L->global("f1");
  double res = f(21.72).toNumber<double>();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(21.72*21.72, res, 1e-10);
  
  // test 2 argument function
  // via LuaProxyGlobal
  L->global("f2") = L->function(sum);
  CPPUNIT_ASSERT(L->global("f2")(4, 5) == 14);
}

void FunctionTest::passingState() {
  L->global("f") = L->function(nt);
  L->doString("temp = f(3, \"test\")[3]");
  CPPUNIT_ASSERT(L->global("temp") == "test");
}
