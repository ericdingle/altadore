#ifndef CHAPARRAL_JS_OBJECT_OPERATOR_
#define CHAPARRAL_JS_OBJECT_OPERATOR_

#include <map>
#include "chaparral/parse/prefix_symbol.h"

class ObjectOperator : public PrefixSymbol {
public:
  ObjectOperator(const Token* token);
  ~ObjectOperator();

  bool Parse(Parser* parser, SymbolRef* root);

  typedef std::map<std::string, SymbolRef> SymbolRefMap;
  const SymbolRefMap& symbol_map() const { return symbol_map_; }

private:
  ObjectOperator(const ObjectOperator&);
  void operator=(const ObjectOperator&);

  SymbolRefMap symbol_map_;
};

#endif
