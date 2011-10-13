#ifndef CHAPARRAL_VALUE_STRING_VALUE_
#define CHAPARRAL_VALUE_STRING_VALUE_

#include "chaparral/value/value.h"

class StringValue : public Value {
public:
  StringValue(std::string str="");
  ~StringValue();

  std::string Get() const;
  std::string ToString() const;

private:
  StringValue(const StringValue&);
  void operator=(const StringValue&);

  std::string str_;
};

#endif
