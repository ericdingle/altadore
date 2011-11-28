#include "chaparral/lexer/token_stream.h"

#include "bonavista/logging/assert.h"
#include "chaparral/lexer/lexer.h"

TokenStream::TokenStream(const Lexer* lexer, const char* input)
    : lexer_(lexer), input_(input), index_(0), position_(1, 1) {
  DASSERT(lexer != NULL);
  DASSERT(input != NULL);
}

TokenStream::~TokenStream() {
}

bool TokenStream::GetNextToken(const Token** token) {
  DASSERT(token != NULL);

  // Return the next token if we already have it.
  if (next_token_.ptr() != NULL) {
    *token = next_token_.Release();
    return true;
  }

  // Consume the white space.
  while (!EndOfInput()) {
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

  // Make sure we're not at the end of the input.
  if (EndOfInput()) {
    error_ = "Unexpected end of input";
    return false;
  }

  // Get the token.
  int type;
  std::string value;
  int count;
  if (!lexer_->GetToken(input_, index_, &type, &value, &count, &error_))
    return false;

  *token = new Token(type, value, position_);

  // Increment the index and position.
  index_ += count;
  position_.column += count;

  return true;
}

bool TokenStream::PeekNextToken(const Token** token) {
  if (next_token_.ptr() == NULL)
    if (!GetNextToken(next_token_.Receive()))
      return false;

  *token = next_token_.ptr();
  return true;
}

bool TokenStream::EndOfInput() {
  return index_ == input_.size() && next_token_.ptr() == NULL;
}

const Token::Position& TokenStream::position() const {
  return position_;
}

const std::string& TokenStream::error() const {
  return error_;
}
