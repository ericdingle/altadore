#ifndef CHAPARRAL_PARSE_STRING_SYMBOL_H_
#define CHAPARRAL_PARSE_STRING_SYMBOL_H_

#include "chaparral/parse/prefix_symbol.h"

class StringSymbol : public PrefixSymbol {
public:
  StringSymbol(const Token* token);
  ~StringSymbol();

  bool Parse(Parser* parser, SymbolRef* root);

private:
  StringSymbol(const StringSymbol&);
  void operator=(const StringSymbol&);
};

#endif

