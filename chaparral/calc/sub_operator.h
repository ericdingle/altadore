#ifndef CHAPARRAL_CALC_SUB_OPERATOR_H_
#define CHAPARRAL_CALC_SUB_OPERATOR_H_

#include "chaparral/calc/binary_operator.h"

class SubOperator : public BinaryOperator {
public:
  static const int BINDING_POWER;

  SubOperator(const Token* token);
  ~SubOperator();

protected:
  double Execute(double left, double right) const;

private:
  SubOperator(const SubOperator&);
  void operator=(const SubOperator&);
};

#endif
