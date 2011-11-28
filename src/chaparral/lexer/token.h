#ifndef CHAPARRAL_LEXER_TOKEN_H_
#define CHAPARRAL_LEXER_TOKEN_H_

#include <string>
#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"

class Token {
 public:
  struct Position {
    Position(uint lint, uint column);

    uint line;
    uint column;
  };

  Token(int type, const std::string& value, const Position& position);
  ~Token();

  bool IsType(int type) const;

  int type() const;
  const std::string& value() const;
  const Position& position() const;

 private:
  const int type_;
  const std::string value_;
  const Position position_;

  DISALLOW_COPY_AND_ASSIGN(Token);
};

#endif
