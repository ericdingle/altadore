#ifndef CHAPARRAL_CALC_DIV_OPERATOR_H_
#define CHAPARRAL_CALC_DIV_OPERATOR_H_

#include "chaparral/calc/binary_operator.h"

class DivOperator : public BinaryOperator {
public:
  static const int BINDING_POWER;

  DivOperator(const Token* token);
  ~DivOperator();

protected:
  double Execute(double left, double right) const;

private:
  DivOperator(const DivOperator&);
  void operator=(const DivOperator&);
};

#endif
