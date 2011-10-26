#include "chaparral/calc/binary_operator.h"

#include "chaparral/execute/executer.h"
#include "chaparral/parse/parser.h"
#include "chaparral/value/number_value.h"

using memory::scoped_ref;

BinaryOperator::BinaryOperator(const Token* token, uint binding_power) : InfixSymbol(token), binding_power_(binding_power) {
}

BinaryOperator::~BinaryOperator() {
}

bool BinaryOperator::Parse(Parser* parser, const Symbol* left, SymbolRef* root) {
  left_ = left;
  SYMBOL_PARSE_ASSERT(left_->IsType(Token::NUMBER) || left_->IsType(Token::OPERATOR), left_, "Unexpected token %s", left_->value().c_str());

  if (!parser->Expression(&right_, binding_power_))
    return false;
  SYMBOL_PARSE_ASSERT(right_->IsType(Token::NUMBER) || right_->IsType(Token::OPERATOR), right_, "Unexpected token %s", right_->value().c_str());

  *root = this;
  return true;
}

bool BinaryOperator::Execute(Executer* executer, scoped_ref<Value>* result) const {
  scoped_ref<Value> value;
  if (!executer->Execute(left_.ptr(), &value))
    return false;
  NumberValue* num_val = dynamic_cast<NumberValue*>(value.ptr());
  double left = num_val->Get();

  if (!executer->Execute(right_.ptr(), &value))
    return false;
  num_val = dynamic_cast<NumberValue*>(value.ptr());
  double right = num_val->Get();

  *result = new NumberValue(Execute(left, right));
  return true;
}
