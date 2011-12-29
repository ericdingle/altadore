#ifndef CHAPARRAL_VALUE_BOOLEAN_VALUE_H_
#define CHAPARRAL_VALUE_BOOLEAN_VALUE_H_

#include "bonavista/util/macros.h"
#include "chaparral/value/value.h"

class BooleanValue : public Value {
 public:
  BooleanValue(bool b);
  virtual ~BooleanValue();

  virtual bool GetAsBoolean(bool* value) const;

 private:
  bool b_;

  DISALLOW_COPY_AND_ASSIGN(BooleanValue);
};

#endif
