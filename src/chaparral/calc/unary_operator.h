#ifndef CHAPARRAL_CALC_UNARY_OPERATOR_H_
#define CHAPARRAL_CALC_UNARY_OPERATOR_H_

#include "chaparral/execute/expression.h"
#include "chaparral/parse/prefix_symbol.h"

class UnaryOperator : public PrefixSymbol, public Expression {
public:
  UnaryOperator(const Token* token, uint binding_power);
  ~UnaryOperator();

  bool Parse(Parser* parser, SymbolRef* root);
  bool Execute(Executer* executer, memory::scoped_refptr<Value>* result) const;

protected:
  virtual double Execute(double right) const=0;

private:
  UnaryOperator(const UnaryOperator&);
  void operator=(const UnaryOperator&);

  uint binding_power_;
  SymbolRef right_;
};

#endif
