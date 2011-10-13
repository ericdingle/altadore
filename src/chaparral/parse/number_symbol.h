#ifndef CHAPARRAL_PARSE_NUMBER_SYMBOL_H_
#define CHAPARRAL_PARSE_NUMBER_SYMBOL_H_

#include "chaparral/parse/prefix_symbol.h"

class NumberSymbol : public PrefixSymbol {
public:
  NumberSymbol(const Token* token);
  ~NumberSymbol();

  bool Parse(Parser* parser, SymbolRef* root);

private:
  NumberSymbol(const NumberSymbol&);
  void operator=(const NumberSymbol&);
};

#endif

