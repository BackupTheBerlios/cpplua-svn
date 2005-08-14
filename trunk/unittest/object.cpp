#include "object.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ObjectTest);

void ObjectTest::setUp() {
  L = new LuaState();
}

void ObjectTest::tearDown() {
  delete L;
}

void ObjectTest::globalsAccess() {
  LuaObject x = L->global("print");
  CPPUNIT_ASSERT(x.isFunction());
}

void ObjectTest::tableLookup() {
  L->doString("t = {} t[7] = 12");
  LuaObject t = L->global("t");
  L->doString("t = nil");
  
  CPPUNIT_ASSERT(t[7] == 12);
}

void ObjectTest::proxyObjects() {
  L->doString("t = {}");
  LuaObject t = L->global("t");
  t[7] = 12;
  CPPUNIT_ASSERT(t[7] == 12);
  t["hello"] = "world";
  CPPUNIT_ASSERT(t["hello"] == "world");
  CPPUNIT_ASSERT(t[7].isNumber());
  CPPUNIT_ASSERT(t["hello"].type() == StringType);
}
