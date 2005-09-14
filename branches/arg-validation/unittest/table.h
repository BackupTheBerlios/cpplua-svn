#ifndef CPPLUA_TEST_TABLE_H
#define CPPLUA_TEST_TABLE_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>

#include "../cpplua.h"

using namespace cpplua;

class TableTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TableTest);
  CPPUNIT_TEST(nested);
  CPPUNIT_TEST(nestedAccess);
  CPPUNIT_TEST_SUITE_END();
private:
  LuaState* L;
public:
  void setUp();
  void tearDown();
  
  // nested tables
  void nested();
  
  // nested accesses
  void nestedAccess();
  
};

#endif
