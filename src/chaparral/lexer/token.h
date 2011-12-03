#ifndef CHAPARRAL_LEXER_TOKEN_H_
#define CHAPARRAL_LEXER_TOKEN_H_

#include <string>
#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"

class Token {
 public:
  struct Position {
    Position(uint line, uint column);

    uint line;
    uint column;
  };

  Token(uint type, const std::string& value, const Position& position);
  ~Token();

  bool IsType(uint type) const;

  uint type() const;
  const std::string& value() const;
  const Position& position() const;

 private:
  const uint type_;
  const std::string value_;
  const Position position_;

  DISALLOW_COPY_AND_ASSIGN(Token);
};

#endif
