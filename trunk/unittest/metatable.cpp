#include "metatable.h"

LuaObject identityIndex(LuaObject table, LuaObject key) {
  return key;
}

CPPUNIT_TEST_SUITE_REGISTRATION(MetatableTest);

void MetatableTest::setUp() {
  L = new LuaState();
}

void MetatableTest::tearDown() {
  delete L;
}

void MetatableTest::set() {
  // set metatable with __index metamethod
  LuaObject t = L->emptyTable();
  LuaObject mt = L->emptyTable();
  mt["__index"] = L->function(identityIndex);
  t.setMetatable(mt);
  t["hello"] = 4;
  
  CPPUNIT_ASSERT(t["hello"] == 4);
  CPPUNIT_ASSERT(t["world"] == "world");
}

void MetatableTest::setAndGet() {
  LuaObject t = L->emptyTable();
  LuaObject mt = L->emptyTable();
  mt["test"] = 21;
  t.setMetatable(mt);
  
  CPPUNIT_ASSERT(t.getMetatable()["test"] == 21);
}
