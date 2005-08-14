#ifndef CPPLUA_TEST_STATE_H
#define CPPLUA_TEST_STATE_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>

#include "../luastate.h"
using namespace cpplua;

// an old-fashioned Lua C function
int square(lua_State*);

class StateTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(StateTest);
  CPPUNIT_TEST(basic);
  CPPUNIT_TEST(userdataPush);
  CPPUNIT_TEST(tables);
  CPPUNIT_TEST(numberType);
  CPPUNIT_TEST(nilType);
  CPPUNIT_TEST(functionType);
  CPPUNIT_TEST_SUITE_END();
  
private:
  LuaState* L;
public:
  void setUp();
  void tearDown();

  // basic stack operations
  void basic();
  
  // pushing userdata
  void userdataPush();
  
  // working with tables
  void tables();
  
  // checking number type
  void numberType();
  
  // checking nil type
  void nilType();
  
  // checking function type
  void functionType();
  
  // calling functions
  void functionCall();
};  

#endif
