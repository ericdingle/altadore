#ifndef CHAPARRAL_EXECUTE_EXECUTER_H_
#define CHAPARRAL_EXECUTE_EXECUTER_H_

#include <string>
#include "bonavista/memory/scoped_ref.h"
#include "chaparral/parse/symbol.h"
#include "chaparral/token/token.h"
#include "chaparral/value/map_value.h"

class Executer {
public:
  Executer();
  ~Executer();

  bool Execute(const Symbol* symbol, memory::scoped_ref<Value>* result);
  bool Execute(const SymbolRefList& symbols);

  const MapValue& context() { return context_; }
  const std::string& error_message() { return error_message_; }
  const Token::Location& error_location() { return error_location_; }

private:
  Executer(const Executer&);
  void operator=(const Executer);

  bool ExecuteSimpleSymbol(const Symbol* symbol, memory::scoped_ref<Value>* result) const;

  MapValue context_;
  std::string error_message_;
  Token::Location error_location_;
};

#endif
