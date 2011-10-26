#include "chaparral/calc/div_operator.h"

const int DivOperator::BINDING_POWER = 20;

DivOperator::DivOperator(const Token* token) : BinaryOperator(token, BINDING_POWER) {
}

DivOperator::~DivOperator() {
}

double DivOperator::Execute(double left, double right) const {
  return left / right;
}
