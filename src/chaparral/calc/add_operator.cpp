#include "chaparral/calc/add_operator.h"

const int AddOperator::BINDING_POWER = 10;

AddOperator::AddOperator(const Token* token) : BinaryOperator(token, BINDING_POWER) {
}

AddOperator::~AddOperator() {
}

double AddOperator::Execute(double left, double right) const {
  return left + right;
}
