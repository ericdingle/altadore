#ifndef CHAPARRAL_VALUE_MAP_VALUE_H_
#define CHAPARRAL_VALUE_MAP_VALUE_H_

#include <map>
#include <string>
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/util/macros.h"
#include "chaparral/value/value.h"

class MapValue : public Value {
 public:
  MapValue();
  virtual ~MapValue();

  virtual bool GetAsMap(MapValue** value);

  bool Get(std::string name, Value**) const;
  void Set(std::string name, Value* v);

 private:
  typedef std::map<std::string, memory::scoped_refptr<Value> > ValueMap;
  ValueMap value_map_;

  DISALLOW_COPY_AND_ASSIGN(MapValue);
};

#endif
