#include "chaparral/lexer/token_stream.h"

#include "bonavista/logging/assert.h"
#include "chaparral/lexer/lexer.h"

TokenStream::TokenStream(const Lexer* lexer, const std::string& input)
    : lexer_(lexer), input_(input), index_(0), position_(1, 1) {
  DASSERT(lexer != NULL);
}

TokenStream::~TokenStream() {
}

bool TokenStream::GetNextToken(const Token** token) {
  DASSERT(token != NULL);

  // We've given away the end of input token, so time to throw an error.
  if (index_ > input_.length()) {
    error_ = "Unexpected end of input.";
    return false;
  }

  // Consume the white space.
  while (index_ < input_.length()) {
    const char& c = input_[index_];
    if (c <= ' ') {
      ++index_;
      if (c == '\n') {
        ++position_.line;
        position_.column = 1;
      } else {
        ++position_.column;
      }
    } else {
      break;
    }
  }

  // If we've reached the end, we return the end of input token.
  if (index_ == input_.length()) {
    *token = new Token(Lexer::TYPE_END_OF_INPUT, "(end of input)", position_);
    ++index_;
    return true;
  }

  // Call into the lexer to get the token.
  int type;
  std::string value;
  uint count;
  if (!lexer_->GetToken(input_, index_, &type, &value, &count, &error_))
    return false;

  *token = new Token(type, value, position_);

  // Increment the index and position.
  index_ += count;
  position_.column += count;

  return true;
}

bool TokenStream::EndOfInput() {
  return index_ == input_.size();
}

const Token::Position& TokenStream::position() const {
  return position_;
}

const std::string& TokenStream::error() const {
  return error_;
}
