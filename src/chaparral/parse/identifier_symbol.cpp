#include "chaparral/parse/identifier_symbol.h"

IdentifierSymbol::IdentifierSymbol(const Token* token) : PrefixSymbol(token) {
}

IdentifierSymbol::~IdentifierSymbol() {
}

bool IdentifierSymbol::Parse(Parser* parser, SymbolRef* root) {
  *root = this;
  return true;
}
