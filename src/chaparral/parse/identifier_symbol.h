#ifndef CHAPARRAL_PARSE_IDENTIFIER_SYMBOL_H_
#define CHAPARRAL_PARSE_IDENTIFIER_SYMBOL_H_

#include "chaparral/parse/prefix_symbol.h"

class IdentifierSymbol : public PrefixSymbol {
public:
  IdentifierSymbol(const Token* token);
  ~IdentifierSymbol();

  bool Parse(Parser* parser, SymbolRef* root);

private:
  IdentifierSymbol(const IdentifierSymbol&);
  void operator=(const IdentifierSymbol&);
};

#endif

