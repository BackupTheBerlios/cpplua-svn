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
  CPPUNIT_TEST(globalFunction);
  CPPUNIT_TEST(luaObjects);
  CPPUNIT_TEST(passingState);
  CPPUNIT_TEST(validation);
  CPPUNIT_TEST(validationPredefined);
  CPPUNIT_TEST_SUITE_END();
private:
  LuaState* L;
public:
  void setUp();
  void tearDown();
  
  // global functions
  void globalFunction();
  
  // functions called as generic LuaObjects
  void luaObjects();
  
  // using the state inside a registered function
  void passingState();
  
  // validation
  void validation();
  
  // validation of predefined lua types
  void validationPredefined();
};

#endif
