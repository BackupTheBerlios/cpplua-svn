#ifndef CPPLUA_TEST_OBJECT_H
#define CPPLUA_TEST_OBJECT_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>

#include "../cpplua.h"

using namespace cpplua;

class ObjectTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(ObjectTest);
  CPPUNIT_TEST_SUITE_END();
private:
  LuaState* L;
public:
  void setUp();
  void tearDown();
  
  // Accessing globals
  void globalsAccess();
  
  // Table lookup
  void tableLookup();
  
  // Proxy objects
  void proxyObjects();
};

#endif
