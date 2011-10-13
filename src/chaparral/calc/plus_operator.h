#ifndef CHAPARRAL_CALC_PLUS_OPERATOR_H_
#define CHAPARRAL_CALC_PLUS_OPERATOR_H_

#include "chaparral/calc/unary_operator.h"

class PlusOperator : public UnaryOperator {
public:
  static const int BINDING_POWER;

  PlusOperator(const Token* token);
  ~PlusOperator();

protected:
  double Execute(double right) const;

private:
  PlusOperator(const PlusOperator&);
  void operator=(const PlusOperator&);
};

#endif
