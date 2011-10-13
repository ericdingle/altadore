#include "chaparral/calc/plus_operator.h"

const int PlusOperator::BINDING_POWER = 30;

PlusOperator::PlusOperator(const Token* token) : UnaryOperator(token, BINDING_POWER) {
}

PlusOperator::~PlusOperator() {
}

double PlusOperator::Execute(double right) const {
  return right;
}
