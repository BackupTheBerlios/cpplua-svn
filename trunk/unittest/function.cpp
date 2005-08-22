#include "function.h"

double square(double x) {
  return x*x;
}

void setGlobal2(LuaState* L, const char* name, LuaObject obj) {
  L->pushString(name);
  obj.push();
  L->setTable(LUA_GLOBALSINDEX);
}

CPPUNIT_TEST_SUITE_REGISTRATION(FunctionTest);

void FunctionTest::setUp() {
  L = new LuaState();
}

void FunctionTest::tearDown() {
  delete L;
}

void FunctionTest::globalFunction() {

}
