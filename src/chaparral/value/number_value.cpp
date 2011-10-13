#include "chaparral/value/number_value.h"

#include "bonavista/string/format.h"

NumberValue::NumberValue(double d) : Value(), d_(d) {
}

NumberValue::~NumberValue() {
}

double NumberValue::Get() const {
  return d_;
}

std::string NumberValue::ToString() const {
  return string::Format("%g", d_);
}
