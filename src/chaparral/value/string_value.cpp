#include "chaparral/value/string_value.h"

#include "bonavista/logging/assert.h"

StringValue::StringValue(const std::string& str) : Value(Value::TYPE_STRING), str_(str) {
}

StringValue::~StringValue() {
}

bool StringValue::GetAsString(std::string* value) const {
  ASSERT(value);
  *value = str_;
  return true;
}
