#ifndef CHAPARRAL_JS_ARRAY_OPERATOR_
#define CHAPARRAL_JS_ARRAY_OPERATOR_

#include <vector>
#include "chaparral/parse/prefix_symbol.h"

class ArrayOperator : public PrefixSymbol {
public:
  ArrayOperator(const Token* token);
  ~ArrayOperator();

  bool Parse(Parser* parser, SymbolRef* root);

  const SymbolRefList& symbols() const { return symbols_; }

private:
  ArrayOperator(const ArrayOperator&);
  void operator=(const ArrayOperator&);

  SymbolRefList symbols_;
};

#endif
