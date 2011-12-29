#ifndef CHAPARRAL_VALUE_VALUE_H_
#define CHAPARRAL_VALUE_VALUE_H_

#include <string>
#include "bonavista/util/macros.h"

class Value {
 public:
  enum Type {
    TYPE_BOOLEAN,
    TYPE_NULL,
    TYPE_NUMBER,
    TYPE_STRING
  };

  explicit Value(Type type);
  virtual ~Value();

  Type type() const;
  bool IsType(Type type) const;

  virtual bool GetAsBoolean(bool* value) const;
  virtual bool GetAsNumber(double* value) const;
  virtual bool GetAsString(std::string* value) const;

 private:
  Type type_;

  DISALLOW_COPY_AND_ASSIGN(Value);
};

#endif
