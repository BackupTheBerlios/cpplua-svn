#include "table.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TableTest);

void TableTest::setUp() {
  L = new LuaState();
}

void TableTest::tearDown() {
  delete L;
}

void TableTest::nested() {
  LuaObject x = L->emptyTable();
  x[0] = L->emptyTable();
  x[0][0] = "nested";
  x[0][x] = "cyclic reference";
  
  CPPUNIT_ASSERT(x[0].isTable());
  CPPUNIT_ASSERT(x[0][0] == "nested");
  CPPUNIT_ASSERT(x[0][x] == "cyclic reference");
}

void TableTest::nestedAccess() {
  LuaObject x = L->emptyTable();
  
  x[0] = 5;
  x[x[0]] = "hello";
  x[x[x[0]]] = 0;
  CPPUNIT_ASSERT(x[5] == "hello");
  CPPUNIT_ASSERT(x[x["hello"]] == 5);
}
