#ifndef CPPLUA_TEST_FUNCTION_H
#define CPPLUA_TEST_FUNCTION_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>

#include "../cpplua.h"

using namespace cpplua;

class FunctionTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(FunctionTest);
  CPPUNIT_TEST_SUITE_END();
private:
  LuaState* L;
public:
  void setUp();
  void tearDown();
  
  void globalFunction();
};

#endif