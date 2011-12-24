#include "chaparral/calc/unary_operator.h"

#include "chaparral/execute/executer.h"
#include "chaparral/parse/parser.h"
#include "chaparral/value/number_value.h"

using memory::scoped_refptr;

UnaryOperator::UnaryOperator(const Token* token, uint binding_power) : PrefixSymbol(token), binding_power_(binding_power) {
}

UnaryOperator::~UnaryOperator() {
}

bool UnaryOperator::Parse(Parser* parser, SymbolRef* root) {
  if (!parser->Expression(&right_, binding_power_))
    return false;
  SYMBOL_PARSE_ASSERT(right_->IsType(Token::NUMBER) || right_->IsType(Token::OPERATOR), right_, "Unexpected token %s", right_->value().c_str());

  *root = this;
  return true;
}

bool UnaryOperator::Execute(Executer* executer, scoped_refptr<Value>* result) const {
  scoped_refptr<Value> value;
  if (!executer->Execute(right_.ptr(), &value))
    return false;
  NumberValue* num_val = dynamic_cast<NumberValue*>(value.ptr());
  double number = num_val->Get();

  *result = new NumberValue(Execute(number));
  return true;
}
