#ifndef LUAOBJECT_H
#define LUAOBJECT_H

namespace cpplua {

class LuaState;

class LuaObject : public LuaIObject {
public:
  explicit LuaObject(const LuaState& L) : LuaIObject(L) {}
};

};

#endif // LUAOBJECT_H
