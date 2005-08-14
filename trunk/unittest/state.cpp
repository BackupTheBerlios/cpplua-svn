#include "state.h"

int square(lua_State* L) {
  double x = static_cast<double>(lua_tonumber(L, -1));
  lua_pop(L, 1);
  lua_pushnumber(L, x*x);
  return 1;
}

// ----

CPPUNIT_TEST_SUITE_REGISTRATION(StateTest);

void StateTest::setUp() {
  L = new LuaState();
}

void StateTest::tearDown() {
  delete L;
}

void StateTest::basic() {
  int n = L->getTop();
  L->pushNumber(7);
  CPPUNIT_ASSERT(L->toNumber<int>() == 7);
  L->pop();
  CPPUNIT_ASSERT(n == L->getTop());
}

void StateTest::userdataPush() {
  int a = 4;
  int* x = &a;
  int n = L->getTop();
  L->pushLightUserdata<int>(x);
  
  CPPUNIT_ASSERT(L->toUserdata<int>() == x);
  L->pop();
  CPPUNIT_ASSERT(n == L->getTop());
}

void StateTest::tables() {
  const int N = 10;
  int n = L->getTop();
  L->newTable();
  for (int i = 0; i < N; i++) {
    L->pushNumber(i);
    L->pushNumber(i);
    L->setTable();
  }
  
  L->pushNil();
  while (L->next()) {
    CPPUNIT_ASSERT(L->toNumber<int>(-1) == L->toNumber<int>(-2));
    L->pop();
  }
  L->pop();
  
  CPPUNIT_ASSERT(n == L->getTop());
}

void StateTest::numberType() {
  L->pushNumber(6);
  CPPUNIT_ASSERT(L->type() == NumberType);
  CPPUNIT_ASSERT(L->isNumber());
  CPPUNIT_ASSERT(L->isString()); // an integer is a string, too
  L->pop();
}

void StateTest::nilType() {
  L->pushNil();
  CPPUNIT_ASSERT(L->type() == NilType);
  CPPUNIT_ASSERT(L->isNil());
  L->pop();
}

void StateTest::functionType() {
  L->pushCFunction(square);
}

void StateTest::functionCall() {
  srand(time(0));
  const int N = 100;
  
  for(int i = 0; i < N; ++i) {
    L->pushCFunction(square);
    double x = (static_cast<double>(rand()) / RAND_MAX)*1000;
    L->pushNumber(x);
    L->pcall(1, 1, 0);
    CPPUNIT_ASSERT(L->toNumber<double>() == x*x);
    L->pop();
  }
}
