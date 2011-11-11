#include "chaparral/value/string_value.h"

#include "bonavista/string/format.h"

StringValue::StringValue(std::string str) : Value(), str_(str) {
}

StringValue::~StringValue() {
}

std::string StringValue::Get() const {
  return str_;
}

std::string StringValue::ToString() const {
  return string::Format("\"%s\"", str_.c_str());
}
