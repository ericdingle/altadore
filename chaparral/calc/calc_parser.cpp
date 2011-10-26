#include "chaparral/calc/calc_parser.h"

#include "chaparral/calc/add_operator.h"
#include "chaparral/calc/div_operator.h"
#include "chaparral/calc/minus_operator.h"
#include "chaparral/calc/mul_operator.h"
#include "chaparral/calc/paren_operator.h"
#include "chaparral/calc/plus_operator.h"
#include "chaparral/calc/sub_operator.h"

CalcParser::CalcParser() : Parser() {
  AddSymbol("+", &InfixSymbol::Create<AddOperator>, AddOperator::BINDING_POWER);
  AddSymbol("/", &InfixSymbol::Create<DivOperator>, DivOperator::BINDING_POWER);
  AddSymbol("*", &InfixSymbol::Create<MulOperator>, MulOperator::BINDING_POWER);
  AddSymbol("-", &InfixSymbol::Create<SubOperator>, SubOperator::BINDING_POWER);

  AddSymbol("-", &PrefixSymbol::Create<MinusOperator>);
  AddSymbol("(", &PrefixSymbol::Create<ParenOperator>);
  AddSymbol("+", &PrefixSymbol::Create<PlusOperator>);
}

CalcParser::~CalcParser() {
}
