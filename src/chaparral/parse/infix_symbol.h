#ifndef CHAPARRAL_PARSE_INFIX_SYMBOL_H_
#define CHAPARRAL_PARSE_INFIX_SYMBOL_H_

#include "chaparral/parse/symbol.h"

class Parser;

class InfixSymbol : public Symbol {
public:
  template <typename T>
  static InfixSymbol* Create(const Token* token) {
    return new T(token);
  }

  InfixSymbol(const Token* token);
  virtual ~InfixSymbol();

  virtual bool Parse(Parser* parser, const Symbol* left, SymbolRef* root)=0;

private:
  InfixSymbol(const InfixSymbol&);
  void operator=(const InfixSymbol&);
};

typedef memory::scoped_ref<InfixSymbol> InfixSymbolRef;

#endif
