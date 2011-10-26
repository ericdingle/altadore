#ifndef CHAPARRAL_VALUE_VALUE_
#define CHAPARRAL_VALUE_VALUE_

#include <string>
#include "bonavista/memory/ref_count.h"

class Value : public memory::RefCount {
public:
  Value();
  virtual ~Value();

  virtual std::string ToString() const=0;

private:
  Value(const Value&);
  void operator=(const Value&);
};

#endif
