#ifndef CHAPARRAL_VALUE_NUMBER_VALUE_
#define CHAPARRAL_VALUE_NUMBER_VALUE_

#include "chaparral/value/value.h"

class NumberValue : public Value {
public:
  NumberValue(double d=0.0);
  ~NumberValue();

  double Get() const;
  std::string ToString() const;

private:
  NumberValue(const NumberValue&);
  void operator=(const NumberValue&);

  double d_;
};

#endif
