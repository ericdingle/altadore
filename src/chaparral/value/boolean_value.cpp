#include "chaparral/value/boolean_value.h"

#include "bonavista/logging/assert.h"

BooleanValue::BooleanValue(bool b) : Value(Value::TYPE_BOOLEAN), b_(b) {
}

BooleanValue::~BooleanValue() {
}

bool BooleanValue::GetAsBoolean(bool* value) const {
  ASSERT(value);
  *value = b_;
  return true;
}
