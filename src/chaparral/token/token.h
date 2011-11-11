#ifndef CHAPARRAL_TOKEN_TOKEN_H_
#define CHAPARRAL_TOKEN_TOKEN_H_

#include <string>
#include "bonavista/base/types.h"

class Token {
public:
  enum Type {
    IDENTIFIER,
    NUMBER,
    STRING,
    OPERATOR,
  };

  struct Location {
    uint line;
    uint column;
  };

  Token(Type type, const std::string& value, uint line, uint column);
  ~Token();

  bool IsType(Type type) const { return type_ == type; }

  Type type() const { return type_; }
  const std::string& value() const { return value_; }
  const Location& location() const { return location_; }

private:
  Token(const Token&);
  void operator=(const Token&);

  Type type_;
  std::string value_;
  Location location_;
};

#endif
