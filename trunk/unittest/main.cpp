#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#ifdef _DEBUG
#include <iostream>
#include <fstream>
#include "../luastate.h"
#endif

int main( int argc, char **argv)
{
  std::ofstream loggerStream(argc > 1 ? argv[1] : "log.txt");
  cpplua::LuaState::setLoggerStream(&loggerStream);

  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  int res = runner.run( "", false ) ? 0 : 1;
  
  return res;
}
