#include "chaparral/calc/minus_operator.h"

const int MinusOperator::BINDING_POWER = 30;

MinusOperator::MinusOperator(const Token* token) : UnaryOperator(token, BINDING_POWER) {
}

MinusOperator::~MinusOperator() {
}

double MinusOperator::Execute(double right) const {
  return -right;
}
