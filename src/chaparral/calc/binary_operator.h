#ifndef CHAPARRAL_CALC_BINARY_OPERATOR_H_
#define CHAPARRAL_CALC_BINARY_OPERATOR_H_

#include "chaparral/execute/expression.h"
#include "chaparral/parse/infix_symbol.h"

class BinaryOperator : public InfixSymbol, public Expression {
public:
  BinaryOperator(const Token* token, uint binding_power_);
  virtual ~BinaryOperator();

  bool Parse(Parser* parser, const Symbol* left, SymbolRef* root);
  bool Execute(Executer* executer, memory::scoped_refptr<Value>* result) const;

protected:
  virtual double Execute(double left, double right) const=0;

private:
  BinaryOperator(const BinaryOperator&);
  void operator=(const BinaryOperator&);

  uint binding_power_;
  SymbolRef left_;
  SymbolRef right_;
};

#endif

