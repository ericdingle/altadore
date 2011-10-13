#include "chaparral/calc/paren_operator.h"

#include "chaparral/parse/parser.h"

ParenOperator::ParenOperator(const Token* token) : PrefixSymbol(token) {
}

ParenOperator::~ParenOperator() {
}

bool ParenOperator::Parse(Parser* parser, SymbolRef* root) {
  if (!parser->Expression(root))
    return false;
  if (!parser->Consume(")"))
    return false;
  return true;
}
