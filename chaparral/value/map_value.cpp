#include "chaparral/value/map_value.h"

MapValue::MapValue() : Value() {
}

MapValue::~MapValue() {
}

memory::scoped_ref<Value> MapValue::Get(std::string name) const {
  ValueMap::const_iterator iter = value_map_.find(name);
  if (iter != value_map_.end()) {
    return iter->second;
  }

  return NULL;
}

void MapValue::Set(std::string name, Value* value) {
  value_map_[name] = value;
}

std::string MapValue::ToString() const {
  return "Map";
}
