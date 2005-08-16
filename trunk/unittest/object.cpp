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
  L->doString("x = 3");
  x = L->global("x");
  CPPUNIT_ASSERT(x.isNumber());
  CPPUNIT_ASSERT(x.toNumber<int>() == 3);
  CPPUNIT_ASSERT(x == 3);
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

void ObjectTest::basicTable() {
  LuaObject x = L->emptyTable();
  x[4] = "hello world";
  LuaObject y = L->emptyTable();
  x[y] = "another table";
  x["test"] = -5;
  
  CPPUNIT_ASSERT(x.isTable());
  CPPUNIT_ASSERT(x.type() == TableType);
  CPPUNIT_ASSERT(x[1].isNil());
  CPPUNIT_ASSERT(x[4] == "hello world");
  CPPUNIT_ASSERT(x[y] == "another table");
  CPPUNIT_ASSERT(x["test"] == -5);
}

void ObjectTest::primitive() {
  LuaObject x = L->primitive(5);
  CPPUNIT_ASSERT(x == 5);
  x = "hello";
  CPPUNIT_ASSERT(x == "hello");
}
