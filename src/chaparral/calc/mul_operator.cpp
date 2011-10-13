#include "chaparral/calc/mul_operator.h"

const int MulOperator::BINDING_POWER = 20;

MulOperator::MulOperator(const Token* token) : BinaryOperator(token, BINDING_POWER) {
}

MulOperator::~MulOperator() {
}

double MulOperator::Execute(double left, double right) const {
  return left * right;
}
