#ifndef CHAPARRAL_PARSER_PARSER_H_
#define CHAPARRAL_PARSER_PARSER_H_

#include <string>
#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"
#include "bonavista/memory/scoped_ptr.h"
#include "chaparral/lexer/token.h"

class ASTNode;
class TokenStream;

class Parser {
 public:
  Parser();
  virtual ~Parser();

  bool Parse(TokenStream* token_stream, ASTNode* root);

  const Token::Position& position() const;
  const std::string& error() const;

 protected:
  bool Parse(ASTNode* root);
  bool GetNextToken(const Token** token);
  bool ParseExpression(uint binding_power, const ASTNode** root);
  bool ConsumeToken(int type);

  virtual uint GetBindingPower(int type) const =0;
  // |token| is owned by the callee.
  virtual bool ParsePrefixToken(const Token* token, const ASTNode** root) =0;
  // |token| and |left| are owned by the callee.
  virtual bool ParseInfixToken(const Token* token, const ASTNode* left,
                               const ASTNode** root) =0;

  memory::scoped_ptr<const Token> look_ahead_token_;

  Token::Position position_;
  std::string error_;

 private:
  TokenStream* token_stream_;

  DISALLOW_COPY_AND_ASSIGN(Parser);
};

#endif
