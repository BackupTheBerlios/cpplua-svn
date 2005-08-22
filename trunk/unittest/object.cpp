#include "object.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ObjectTest);

void ObjectTest::setUp() {
  L = new LuaState();
}

void ObjectTest::tearDown() {
  delete L;
}

// a dummy class to test userdata
class Dummy {
  int data;
public:
  Dummy(int data = 0) : data(data) {}
  bool operator==(const Dummy& d) { return data == d.data; }
};

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

void ObjectTest::conversions() {
  LuaObject x = L->primitive(5);
  int x_int = x.toNumber<int>();
  CPPUNIT_ASSERT(x_int == 5);
  CPPUNIT_ASSERT(x == x_int);
  
  LuaObject y = L->primitive("hello");
  const char* y_str = y.toString();
  CPPUNIT_ASSERT(strcmp(y_str, "hello") == 0);
  CPPUNIT_ASSERT(y == y_str);
  
  Dummy* a = new Dummy(37);
  LuaObject z = L->primitive(a);
  Dummy a2 = *z.toUserdata<Dummy>(); // default copy constructor used here
  CPPUNIT_ASSERT(z == a);
  CPPUNIT_ASSERT(a != &a2);
  CPPUNIT_ASSERT(*a == a2);
  delete a;
}
