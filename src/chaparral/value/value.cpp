#include "chaparral/value/value.h"

Value::Value(Type type) : type_(type) {
}

Value::~Value() {
}

Value::Type Value::type() const {
  return type_;
}

bool Value::IsType(Value::Type type) const {
  return type_ == type;
}

bool Value::GetAsBoolean(bool* value) const {
  return false;
}

bool Value::GetAsMap(MapValue** value) {
  return false;
}

bool Value::GetAsNumber(double* value) const {
  return false;
}

bool Value::GetAsString(std::string* value) const {
  return false;
}
