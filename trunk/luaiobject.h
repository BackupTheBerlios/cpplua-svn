#ifndef LUAIOBJECT_H
#define LUAIOBJECT_H

namespace cpplua {

/**
 * LuaIObject is a common interface for all Lua objects handled by cpplua.
 */
class LuaIObject {
  LuaState& L;
public:
  explicit LuaIObject(LuaState& L) : L(L) {}
  virtual void push() const = 0;
};

};

#endif
