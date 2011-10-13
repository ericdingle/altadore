#ifndef CHAPARRAL_CALC_MINUS_OPERATOR_H_
#define CHAPARRAL_CALC_MINUS_OPERATOR_H_

#include "chaparral/calc/unary_operator.h"

class MinusOperator : public UnaryOperator {
public:
  static const int BINDING_POWER;

  MinusOperator(const Token* token);
  ~MinusOperator();

protected:
  double Execute(double right) const;

private:
  MinusOperator(const MinusOperator&);
  void operator=(const MinusOperator&);
};

#endif
