#include "chaparral/value/map_value.h"

#include "bonavista/logging/assert.h"

MapValue::MapValue() : Value(Value::TYPE_MAP) {
}

MapValue::~MapValue() {
}

bool MapValue::GetAsMap(MapValue** value) {
  ASSERT(value);
  *value = this;
  return true;
}

bool MapValue::Get(std::string name, Value** value) const {
  ValueMap::const_iterator iter = value_map_.find(name);
  if (iter != value_map_.end()) {
    memory::scoped_refptr<Value> scoped_value(iter->second);
    *value = scoped_value.Release();  // Don't Release() value.
    return true;
  }

  return false;
}

void MapValue::Set(std::string name, Value* value) {
  value_map_[name] = value;
}
