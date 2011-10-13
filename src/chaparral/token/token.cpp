#include "chaparral/token/token.h"

Token::Token(Type type, const std::string& value, uint line, uint column) : type_(type), value_(value) {
  location_.line = line;
  location_.column = column;
}

Token::~Token() {
}
