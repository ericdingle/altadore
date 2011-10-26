#include "chaparral/json/json_parser.h"

#include "chaparral/json/array_operator.h"
#include "chaparral/json/object_operator.h"

JsonParser::JsonParser() {
  AddSymbol("[", &PrefixSymbol::Create<ArrayOperator>);
  AddSymbol("{", &PrefixSymbol::Create<ObjectOperator>);
}

JsonParser::~JsonParser() {
}
