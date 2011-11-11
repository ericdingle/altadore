#ifndef CHAPARRAL_CALC_MUL_OPERATOR_H_
#define CHAPARRAL_CALC_MUL_OPERATOR_H_

#include "chaparral/calc/binary_operator.h"

class MulOperator : public BinaryOperator {
public:
  static const int BINDING_POWER;

  MulOperator(const Token* token);
  ~MulOperator();

protected:
  double Execute(double left, double right) const;

private:
  MulOperator(const MulOperator&);
  void operator=(const MulOperator&);
};

#endif
