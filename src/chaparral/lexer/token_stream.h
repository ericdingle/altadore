#ifndef CHAPARRAL_LEXER_TOKEN_STREAM_H_
#define CHAPARRAL_LEXER_TOKEN_STREAM_H_

#include "bonavista/memory/scoped_ptr.h"
#include "chaparral/lexer/token.h"

class Lexer;

class TokenStream {
 public:
  TokenStream(const Lexer* lexer, const char* input);
  ~TokenStream();

  bool GetNextToken(const Token** token);

  const Token::Position& position() const;
  const std::string& error() const;

 private:
  bool EndOfInput();

  const Lexer* const lexer_;

  const std::string input_;
  uint index_;

  Token::Position position_;
  std::string error_;

  DISALLOW_COPY_AND_ASSIGN(TokenStream);
};

#endif
