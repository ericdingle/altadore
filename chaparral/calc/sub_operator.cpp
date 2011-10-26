#include "chaparral/calc/sub_operator.h"

const int SubOperator::BINDING_POWER = 10;

SubOperator::SubOperator(const Token* token) : BinaryOperator(token, BINDING_POWER) {
}

SubOperator::~SubOperator() {
}

double SubOperator::Execute(double left, double right) const {
  return left - right;
}
