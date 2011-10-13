#include "chaparral/json/object_operator.h"

#include "chaparral/parse/parser.h"

ObjectOperator::ObjectOperator(const Token* token) : PrefixSymbol(token) {
}

ObjectOperator::~ObjectOperator() {
}

bool ObjectOperator::Parse(Parser* parser, SymbolRef* root) {
  while (true) {
    SymbolRef key;
    if (!parser->Expression(&key))
      return false;

    if (!key->IsType(Token::STRING)) {
      parser->set_error_message(string::Format("Expecting string but found token %s", key->value().c_str()));
      parser->set_error_location(key->location());
      return false;
    }

    if (!parser->Consume(":"))
      return false;

    SymbolRef value;
    if (!parser->Expression(&value))
      return false;

    symbol_map_[key->value()] = value;

    if (parser->PeekNextValue() == "}")
      break;

    if (!parser->Consume(","))
      return false;
  }

  if (!parser->Consume("}"))
    return false;

  *root = this;
  return true;
}
