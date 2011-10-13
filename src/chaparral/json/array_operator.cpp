#include "chaparral/json/array_operator.h"

#include "chaparral/parse/parser.h"

ArrayOperator::ArrayOperator(const Token* token) : PrefixSymbol(token) {
}

ArrayOperator::~ArrayOperator() {
}

bool ArrayOperator::Parse(Parser* parser, SymbolRef* root) {
  while (true) {
    SymbolRef symbol;
    if (!parser->Expression(&symbol))
      return false;
    symbols_.push_back(symbol);

    if (parser->PeekNextValue() != ",")
      break;

    if (!parser->Consume(","))
      return false;
  }

  if (!parser->Consume("]"))
    return false;

  *root = this;
  return true;
}
