#include "chaparral/value/number_value.h"

#include "bonavista/logging/assert.h"

NumberValue::NumberValue(double d) : Value(Value::TYPE_NUMBER), d_(d) {
}

NumberValue::~NumberValue() {
}

bool NumberValue::GetAsNumber(double* value) const {
  ASSERT(value);
  *value = d_;
  return true;
}
