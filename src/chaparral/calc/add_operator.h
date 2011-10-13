#ifndef CHAPARRAL_CALC_ADD_OPERATOR_H_
#define CHAPARRAL_CALC_ADD_OPERATOR_H_

#include "chaparral/calc/binary_operator.h"

class AddOperator : public BinaryOperator {
public:
  static const int BINDING_POWER;

  AddOperator(const Token* token);
  ~AddOperator();

protected:
  double Execute(double left, double right) const;

private:
  AddOperator(const AddOperator&);
  void operator=(const AddOperator&);
};

#endif
