#include "chaparral/parse/string_symbol.h"

StringSymbol::StringSymbol(const Token* token) : PrefixSymbol(token) {
}

StringSymbol::~StringSymbol() {
}

bool StringSymbol::Parse(Parser* parser, SymbolRef* root) {
  *root = this;
  return true;
}
