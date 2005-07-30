#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
int current_test;
int ok;
int failed;
vector<string> test_names;
void rassert(bool, const char* = 0, bool = false);

template <typename T1, typename T2>
void rassert_equal(const T1&, const T2&);

template <typename T1, typename T2>
void rassert_equal_float(const T1&, const T2&, double epsilon = 1e-10);

void rassert(bool condition, const char* message, bool msg_on_pass) {
  if (!condition) {
    cout << "ASSERTION FAILED on test" << endl;
    cout << "  " << test_names[current_test] << endl;
    if (message)
      cout << "  " << message << endl;
    failed++;
  }
  else {
    if (message && msg_on_pass) {
      cout << "assertion passed (";
      cout << message << ")" << endl;
    }
    ok++;
  }
}

template <typename T1, typename T2>
void rassert_equal(const T1& x1, const T2& x2) {
  ostringstream msg;
  msg << "expected " << x1 << ", got " << x2;
  rassert(x1 == x2, msg.str().c_str(), false);
}

template <typename T1, typename T2>
void rassert_equal_float(const T1& x1, const T2& x2, double epsilon) {
  ostringstream msg;
  msg << "expected " << x1 << ", got " << x2;
  rassert(abs(x1-x2) < epsilon, msg.str().c_str(), false);
}


#include <stdlib.h>
#include <time.h>
#include "../luastate.h"
using namespace cpplua;
LuaState L;

int square(lua_State* L) {
  double x = static_cast<double>(lua_tonumber(L, -1));
  lua_pop(L, 1);
  lua_pushnumber(L, x*x);
  return 1;
}


#include "../luaobject.h"

void setUp() {
#line 16 "state.rdcut"
}

void tearDown() {
#line 17 "state.rdcut"
}

void test0() {
#line 20 "state.rdcut"
  int n = L.getTop();
  L.pushNumber(7);
  rassert_equal(L.toNumber<int>(), 7);
  L.pop();
  rassert_equal(n, L.getTop());
  
}

void test1() {
#line 27 "state.rdcut"
  int a = 4;
  int* x = &a;
  int n = L.getTop();
  L.pushLightUserdata<int>(x);
  rassert(L.toUserdata<int>() == x);
  L.pop();
  rassert(n == L.getTop());
  
}

void test2() {
#line 36 "state.rdcut"
  int N = 10;
  int n = L.getTop();
  L.newTable();
  for(int i=0; i<N; i++) {
    L.pushNumber(i);
    L.pushNumber(i);
    L.setTable();
  }
  
  L.pushNil();
  while(L.next()) {
    rassert(L.toNumber<int>(-1) == L.toNumber<int>(-2));
    L.pop();
  }
  L.pop();
  rassert_equal(n, L.getTop());
  
}

void test3() {
#line 54 "state.rdcut"
  L.pushString("hello world");
  rassert_equal(L.type(), StringType);
  rassert(L.isString());
  rassert(!L.isNumber());
  L.pop();
  
}

void test4() {
#line 61 "state.rdcut"
  L.pushNumber(6);
  rassert_equal(L.type(), NumberType);
  rassert(L.isNumber());
  rassert(L.isString()); // an integer is a string, too
  L.pop();
  
}

void test5() {
#line 68 "state.rdcut"
  L.pushNil();
  rassert_equal(L.type(), NilType);
  rassert(L.isNil());
  L.pop();
  
}

void test6() {
#line 74 "state.rdcut"
  L.pushCFunction(square);
  rassert_equal(L.type(), FunctionType);
  rassert(L.isFunction());
  L.pop();
  
}

void test7() {
#line 80 "state.rdcut"
  srand(time(0));
  
  for(int i=0; i<100; i++) {
    L.pushCFunction(square);
    double x = (static_cast<double>(rand())/RAND_MAX)*100;
    L.pushNumber(x);
    L.pcall(1,1,0);
    rassert_equal_float(L.toNumber<double>(), x*x);
    L.pop();
  }
  
}

void test8() {
#line 4 "object.rdcinc"
  LuaObject x = L.global("print");
  rassert(x.isFunction());
}

void test9() {
#line 7 "object.rdcinc"
  L.doString("t = {}\nt[7] = 12");
  LuaObject t = L.global("t");
  L.doString("t = nil");
  rassert_equal(t[7], 12);
}

void test10() {
#line 12 "object.rdcinc"
  L.doString("t = {}");
  LuaObject t = L.global("t");
  t[7] = 12;
  rassert_equal(t[7], 12);
  t["hello"] = "world";
  rassert_equal(t["hello"],world);
  rassert(t[7].isNumber());
  rassert_equal(t["hello"].type(), StringType);
}

int main() {

  ok = 0;
  failed = 0;
  test_names.resize(11);
test_names[0] = "Basic stack operations";
test_names[1] = "Pushing userdata";
test_names[2] = "Working with tables";
test_names[3] = "Checking string type";
test_names[4] = "Checking number type";
test_names[5] = "Checking nil type";
test_names[6] = "Checking function type";
test_names[7] = "Calling functions";
test_names[8] = "Accessing globals";
test_names[9] = "Table lookup";
test_names[10] = "Proxy objects";



  // fixture initialization
  setUp();

  // Basic stack operations
  current_test = 0;
  test0();
  // Pushing userdata
  current_test = 1;
  test1();
  // Working with tables
  current_test = 2;
  test2();
  // Checking string type
  current_test = 3;
  test3();
  // Checking number type
  current_test = 4;
  test4();
  // Checking nil type
  current_test = 5;
  test5();
  // Checking function type
  current_test = 6;
  test6();
  // Calling functions
  current_test = 7;
  test7();
  // Accessing globals
  current_test = 8;
  test8();
  // Table lookup
  current_test = 9;
  test9();
  // Proxy objects
  current_test = 10;
  test10();

  // fixture destruction
  tearDown();

  cout << " --- DONE ---" << endl;
  cout << ok << " assertions passed" << endl;
  cout << failed << " assertions failed" << endl;

  return 0;
}
