#ifndef CHAPARRAL_PARSE_PARSER_H_
#define CHAPARRAL_PARSE_PARSER_H_

#include <map>
#include <string>
#include "bonavista/util/types.h"
#include "chaparral/parse/infix_symbol.h"
#include "chaparral/parse/prefix_symbol.h"
#include "chaparral/parse/symbol.h"
#include "chaparral/token/token_stream.h"

class Parser {
public:
  Parser();
  virtual ~Parser();

  // Public API.
  typedef PrefixSymbol* (*CreatePrefixSymbol)(const Token*);
  void AddSymbol(std::string value, CreatePrefixSymbol createSymbol);
  typedef InfixSymbol* (*CreateInfixSymbol)(const Token*);
  void AddSymbol(std::string value, CreateInfixSymbol createSymbol, uint binding_power);

  bool Parse(TokenStream* token_stream, SymbolRef* symbol);
  bool Parse(TokenStream* token_stream, SymbolRefList* symbols);

  // Public API to Symbol dervied classes.
  bool Expression(SymbolRef* symbol, uint binding_power=0);
  bool Consume(std::string value);
  std::string PeekNextValue();

  std::string error_message() const { return error_message_; }
  void set_error_message(const std::string& error_message) { error_message_ = error_message; }

  Token::Location error_location() const { return error_location_; }
  void set_error_location(const Token::Location& error_location) { error_location_ = error_location; }

private:
  Parser(const Parser&);
  void operator=(const Parser&);

  bool GetNextToken(const Token** token);
  uint PeekNextBindingPower();

  bool GetNextPrefixSymbol(PrefixSymbolRef* symbol);
  bool GetNextInfixSymbol(InfixSymbolRef* symbol);

  typedef std::map<std::string, CreatePrefixSymbol> PrefixSymbolTable;
  PrefixSymbolTable prefix_symbol_table_;
  typedef std::map<std::string, std::pair<CreateInfixSymbol, uint> > InfixSymbolTable;
  InfixSymbolTable infix_symbol_table_;

  TokenStream* token_stream_;
  std::string error_message_;
  Token::Location error_location_;
};

#endif
