#include "chaparral/lexer/lexer.h"

Lexer::Lexer() {
}

Lexer::~Lexer() {
}

bool Lexer::IsAlpha(const char& c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::IsDigit(const char& c) {
  return c >= '0' && c <= '9';
}
