#ifndef CPPLUA_TEST_METHOD_H
#define CPPLUA_TEST_METHOD_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>

#include "../cpplua.h"

using namespace cpplua;

class MethodTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(MethodTest);
  CPPUNIT_TEST(method);
  CPPUNIT_TEST_SUITE_END();
private:
  LuaState* L;
public:
  void setUp();
  void tearDown();
  
  void method();
};

#endif
