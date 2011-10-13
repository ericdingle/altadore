#ifndef CHAPARRAL_PARSE_PREFIX_SYMBOL_H_
#define CHAPARRAL_PARSE_PREFIX_SYMBOL_H_

#include "chaparral/parse/symbol.h"

class Parser;

class PrefixSymbol : public Symbol {
public:
  template <typename T>
  static PrefixSymbol* Create(const Token* token) {
    return new T(token);
  }

  PrefixSymbol(const Token* token);
  virtual ~PrefixSymbol();

  virtual bool Parse(Parser* parser, SymbolRef* root)=0;

private:
  PrefixSymbol(const PrefixSymbol&);
  void operator=(const PrefixSymbol&);
};

typedef memory::scoped_ref<PrefixSymbol> PrefixSymbolRef;

#endif
