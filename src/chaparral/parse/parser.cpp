#include "chaparral/parse/parser.h"

#include "bonavista/logging/assert.h"
#include "bonavista/string/format.h"
#include "chaparral/parse/identifier_symbol.h"
#include "chaparral/parse/number_symbol.h"
#include "chaparral/parse/string_symbol.h"

using memory::scoped_ptr;

Parser::Parser() : token_stream_(NULL) {
}

Parser::~Parser() {
}

void Parser::AddSymbol(std::string value, CreatePrefixSymbol createPrefixSymbol) {
  prefix_symbol_table_[value] = createPrefixSymbol;
}

void Parser::AddSymbol(std::string value, CreateInfixSymbol createInfixSymbol, uint binding_power) {
  infix_symbol_table_[value] = std::make_pair(createInfixSymbol, binding_power);
}

bool Parser::Parse(TokenStream* token_stream, SymbolRef* symbol) {
  ASSERT(token_stream_ == NULL);
  token_stream_ = token_stream;

  if (!Expression(symbol)) {
    token_stream_ = NULL;
    return false;
  }
  DASSERT(symbol->ptr() != NULL);

  token_stream_ = NULL;
  return true;
}

bool Parser::Parse(TokenStream* token_stream, SymbolRefList* symbols) {
  while (!token_stream->EndOfInput()) {
    SymbolRef symbol;
    if (!Parse(token_stream, &symbol))
      return false;
    symbols->push_back(symbol);
  }

  return true;
}

bool Parser::Expression(SymbolRef* symbol, uint binding_power) {
  ASSERT(token_stream_ != NULL);

  PrefixSymbolRef prefix_symbol;
  if (!GetNextPrefixSymbol(&prefix_symbol))
    return false;

  SymbolRef left;
  if (!prefix_symbol->Parse(this, &left))
    return false;
  DASSERT(left.ptr() != NULL);

  while (binding_power < PeekNextBindingPower()) {
    InfixSymbolRef infix_symbol;
    if (!GetNextInfixSymbol(&infix_symbol))
      return false;
    if (!infix_symbol->Parse(this, left.ptr(), &left))
      return false;
    DASSERT(left.ptr() != NULL);
  }

  *symbol = left;
  return true;
}

bool Parser::Consume(std::string value) {
  ASSERT(token_stream_ != NULL);

  scoped_ptr<const Token> token;
  if (!GetNextToken(token.Receive()))
    return false;

  if (token->value() != value) {
    error_message_ = string::Format("Expecting token %s but found %s", value.c_str(), token->value().c_str());
    error_location_ = token->location();
    return false;
  }

  return true;
}

std::string Parser::PeekNextValue() {
  ASSERT(token_stream_ != NULL);

  const Token* token;
  if (!token_stream_->PeekNextToken(&token))
    return "";

  return token->value();
}

bool Parser::GetNextToken(const Token** token) {
  if (!token_stream_->GetNextToken(token)) {
    error_message_ = token_stream_->error_message();
    error_location_ = token_stream_->error_location();
    return false;
  }

  return true;
}

uint Parser::PeekNextBindingPower() {
  const Token* token;
  if (!token_stream_->PeekNextToken(&token))
    return 0;

  InfixSymbolTable::iterator iter = infix_symbol_table_.find(token->value());
  return iter != infix_symbol_table_.end() ? iter->second.second : 0;
}

bool Parser::GetNextPrefixSymbol(PrefixSymbolRef* symbol) {
  scoped_ptr<const Token> token;
  if (!GetNextToken(token.Receive()))
    return false;

  PrefixSymbolTable::iterator iter = prefix_symbol_table_.find(token->value());
  if (iter != prefix_symbol_table_.end()) {
    *symbol = iter->second(token.Release());
    return true;
  }

  if (token->IsType(Token::NUMBER)) {
    *symbol = new NumberSymbol(token.Release());
    return true;
  } else if (token->IsType(Token::IDENTIFIER)) {
    *symbol = new IdentifierSymbol(token.Release());
    return true;
  } else if (token->IsType(Token::STRING)) {
    *symbol = new StringSymbol(token.Release());
    return true;
  }

  error_message_ = string::Format("Unexpected token %s", token->value().c_str());
  error_location_ = token->location();

  return false;
}

bool Parser::GetNextInfixSymbol(InfixSymbolRef* symbol) {
  scoped_ptr<const Token> token;
  if (!GetNextToken(token.Receive()))
    return false;

  InfixSymbolTable::iterator iter = infix_symbol_table_.find(token->value());
  if (iter != infix_symbol_table_.end()) {
    *symbol = iter->second.first(token.Release());
    return true;
  }

  error_message_ = string::Format("Unexpected token %s", token->value().c_str());
  error_location_ = token->location();

  return false;
}
