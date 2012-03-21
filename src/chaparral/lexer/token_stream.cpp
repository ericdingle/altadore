#include "chaparral/lexer/token_stream.h"

#include "chaparral/lexer/lexer.h"

TokenStream::TokenStream(const Lexer* lexer, const std::string& input)
    : lexer_(lexer), input_(input), index_(0) {
  DCHECK(lexer);
}

TokenStream::~TokenStream() {
}

bool TokenStream::GetNextToken(const Token** token) {
  DCHECK(token);

  // Consume the white space.
  while (HasInput()) {
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

  // If we've reached the end, we return an end of input token.
  if (!HasInput()) {
    *token = new Token(Lexer::TYPE_END_OF_INPUT, "(end of input)", position_);
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

bool TokenStream::HasInput() const {
  return index_ < input_.length();
}

const Token::Position& TokenStream::position() const {
  return position_;
}

const std::string& TokenStream::error() const {
  return error_;
}
