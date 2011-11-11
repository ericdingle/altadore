#include "chaparral/parse/number_symbol.h"

NumberSymbol::NumberSymbol(const Token* token) : PrefixSymbol(token) {
}

NumberSymbol::~NumberSymbol() {
}

bool NumberSymbol::Parse(Parser* parser, SymbolRef* root) {
  *root = this;
  return true;
}
