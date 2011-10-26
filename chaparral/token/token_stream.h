#ifndef CHAPARRAL_TOKEN_TOKE_STREAM_H_
#define CHAPARRAL_TOKEN_TOKE_STREAM_H_

#include <vector>
#include "bonavista/memory/scoped_ptr.h"
#include "chaparral/token/token.h"

class TokenStream {
public:
  TokenStream(const char* input);
  ~TokenStream();

  bool GetNextToken(const Token** token);
  bool PeekNextToken(const Token** token);
  bool EndOfInput();

  const Token::Location& location() const { return location_; }
  const std::string& error_message() const { return error_message_; }
  const Token::Location& error_location() const { return error_location_; }

private:
  TokenStream(const TokenStream&);
  void operator=(const TokenStream&);

  const Token* GetIdentifierToken();
  const Token* GetNumberToken();
  const Token* GetStringToken();
  const Token* GetOperatorToken();

  std::string ReplaceEscapeCharacters(const std::string& str);

  bool IsAlpha(const char& c);
  bool IsDigit(const char& c);

  std::string input_;
  uint index_;
  Token::Location location_;

  memory::scoped_ptr<const Token> next_token_;
  std::string error_message_;
  Token::Location error_location_;
};

#endif
