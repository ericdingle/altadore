#include "chaparral/lexer/token.h"

Token::Position::Position(uint line, uint column) {
  this->line = line;
  this->column = column;
}

Token::Token(uint type, const std::string& value, const Position& position)
    : type_(type), value_(value), position_(position) {
}

Token::~Token() {
}

bool Token::IsType(uint type) const {
  return type_ == type;
}

uint Token::type() const {
  return type_;
}

const std::string& Token::value() const {
  return value_;
}

const Token::Position& Token::position() const {
  return position_;
}
