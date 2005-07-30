#ifndef LUABRACKET_H
#define LUABRACKET_H

namespace cpplua {

template <typename Table, typename Key>
class LuaBracket : public LuaIObject {
  const Table& table;
  const Key& key;
public:
  explicit LuaBracket(const Table& table, const Key& key) :
    table(table),
    key(key),
    LuaIObject(table.getState());
  {}

  void push() const;
  
};

};

#endif LUABRACKET_H
