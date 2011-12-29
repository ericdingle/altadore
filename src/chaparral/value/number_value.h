#ifndef CHAPARRAL_VALUE_NUMBER_VALUE_H_
#define CHAPARRAL_VALUE_NUMBER_VALUE_H_

#include "bonavista/util/macros.h"
#include "chaparral/value/value.h"

class NumberValue : public Value {
 public:
  NumberValue(double d);
  virtual ~NumberValue();

  virtual bool GetAsNumber(double* value) const;

 private:
  double d_;

  DISALLOW_COPY_AND_ASSIGN(NumberValue);
};

#endif
