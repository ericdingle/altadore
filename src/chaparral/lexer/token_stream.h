#ifndef CHAPARRAL_LEXER_TOKEN_STREAM_H_
#define CHAPARRAL_LEXER_TOKEN_STREAM_H_

#include <string>
#include "bonavista/memory/scoped_ptr.h"
#include "chaparral/lexer/token.h"

class Lexer;

class TokenStream {
 public:
  TokenStream(const Lexer* lexer, const std::string& input);
  ~TokenStream();

  bool HasInput() const;
  bool GetNextToken(const Token** token);

  const Token::Position& position() const;
  const std::string& error() const;

 private:
  const Lexer* const lexer_;

  const std::string input_;
  uint index_;

  Token::Position position_;
  std::string error_;

  DISALLOW_COPY_AND_ASSIGN(TokenStream);
};

#endif
