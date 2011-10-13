#ifndef CHAPARRAL_VALUE_MAP_VALUE_
#define CHAPARRAL_VALUE_MAP_VALUE_

#include <map>
#include <string>
#include "bonavista/memory/scoped_ref.h"
#include "chaparral/value/value.h"

class MapValue : public Value {
public:
  MapValue();
  ~MapValue();

  memory::scoped_ref<Value> Get(std::string name) const;
  void Set(std::string name, Value* v);
  std::string ToString() const;

private:
  MapValue(const MapValue&);
  void operator=(const MapValue&);

  typedef std::map<std::string, memory::scoped_ref<Value> > ValueMap;
  ValueMap value_map_;
};

#endif
