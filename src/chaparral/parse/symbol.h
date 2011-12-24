#ifndef CHAPARRAL_PARSE_SYMBOL_H_
#define CHAPARRAL_PARSE_SYMBOL_H_

#include <string>
#include <vector>
#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_ptr.h"
#include "bonavista/memory/scoped_refptr.h"
#include "bonavista/string/format.h"
#include "chaparral/token/token.h"

class Symbol : public memory::RefCount {
public:
  Symbol(const Token* token);
  virtual ~Symbol();

  bool IsType(Token::Type type) const { return token_->IsType(type); }
  Token::Type type() const { return token_->type(); }
  std::string value() const { return token_->value(); }
  Token::Location location() const { return token_->location(); }

private:
  Symbol(const Symbol&);
  void operator=(const Symbol&);

  memory::scoped_ptr<const Token> token_;
};

typedef memory::scoped_refptr<const Symbol> SymbolRef;
typedef std::vector<SymbolRef> SymbolRefList;

#define SYMBOL_PARSE_ASSERT(cond, symbol, msg, ...) \
  do { \
    if (!(cond)) { \
      parser->set_error_message(string::Format(msg, ##__VA_ARGS__)); \
      parser->set_error_location(symbol->location()); \
      return false; \
    } \
  } while (0)

#endif
