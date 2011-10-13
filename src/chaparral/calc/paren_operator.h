#ifndef CHAPARRAL_CALC_PAREN_OPERATOR_H_
#define CHAPARRAL_CALC_PAREN_OPERATOR_H_

#include "chaparral/parse/prefix_symbol.h"

class ParenOperator : public PrefixSymbol {
public:
  ParenOperator(const Token* token);
  ~ParenOperator();

  bool Parse(Parser* parser, SymbolRef* root);

private:
  ParenOperator(const ParenOperator&);
  void operator=(const ParenOperator&);
};

#endif
