#include "chaparral/execute/executer.h"

#include "bonavista/types.h"
#include "bonavista/string/format.h"
#include "chaparral/execute/expression.h"
#include "chaparral/parse/identifier_symbol.h"
#include "chaparral/parse/number_symbol.h"
#include "chaparral/parse/string_symbol.h"
#include "chaparral/value/number_value.h"
#include "chaparral/value/string_value.h"

Executer::Executer() {
}

Executer::~Executer() {
}

bool Executer::Execute(const Symbol* symbol, memory::scoped_ref<Value>* result) {
  if (ExecuteSimpleSymbol(symbol, result))
    return true;

  const Expression* expression = dynamic_cast<const Expression*>(symbol);
  if (expression == NULL) {
    error_message_ = string::Format("Symbol %s cannot be executed.", symbol->value().c_str());
    error_location_ = symbol->location();
    return false;
  }

  if (!expression->Execute(this, result))
    return false;

  return true;
}

bool Executer::Execute(const SymbolRefList& symbols) {
  for (uint i = 0; i < symbols.size(); ++i) {
    memory::scoped_ref<Value> result;
    if (!Execute(symbols[i].ptr(), &result))
      return false;
  }

  return true;
}

bool Executer::ExecuteSimpleSymbol(const Symbol* symbol, memory::scoped_ref<Value>* result) const {
  if (dynamic_cast<const IdentifierSymbol*>(symbol) != NULL) {
    *result = context_.Get(symbol->value());
    return true;
  }

  if (dynamic_cast<const NumberSymbol*>(symbol) != NULL) {
    *result = new NumberValue(atof(symbol->value().c_str()));
    return true;
  }

  if (dynamic_cast<const StringSymbol*>(symbol) != NULL) {
    *result = new StringValue(symbol->value());
    return true;
  }

  return false;
}
