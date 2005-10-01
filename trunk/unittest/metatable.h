#ifndef CPPLUA_TEST_METATABLE_H
#define CPPLUA_TEST_METATABLE_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>

#include "../cpplua.h"

using namespace cpplua;

class MetatableTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(MetatableTest);
  CPPUNIT_TEST(set);
  CPPUNIT_TEST(setAndGet);
  CPPUNIT_TEST_SUITE_END();
private:
  LuaState* L;
public:
  void setUp();
  void tearDown();
  
  void set();
  void setAndGet();
};

#endif
