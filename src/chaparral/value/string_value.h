#ifndef CHAPARRAL_VALUE_STRING_VALUE_H_
#define CHAPARRAL_VALUE_STRING_VALUE_H_

#include "bonavista/util/macros.h"
#include "chaparral/value/value.h"

class StringValue : public Value {
 public:
  StringValue(const std::string& str);
  virtual ~StringValue();

  virtual bool GetAsString(std::string* value) const;

 private:
  std::string str_;

  DISALLOW_COPY_AND_ASSIGN(StringValue);
};

#endif
