#include "chaparral/token/token_stream.h"

#include "bonavista/logging/assert.h"

TokenStream::TokenStream(const char* input) : input_(input), index_(0) {
  location_.line = 1;
  location_.column = 1;
}

TokenStream::~TokenStream() {
}

bool TokenStream::GetNextToken(const Token** token) {
  DASSERT(token != NULL);

  if (next_token_.ptr() != NULL) {
    *token = next_token_.Release();
    return true;
  }

  while (!EndOfInput()) {
    const char& c = input_[index_];
    // Consume white space.
    if (c <= ' ') {
      index_++;
      if (c == '\n') {
        ++location_.line;
        location_.column = 1;
      } else {
        ++location_.column;
      }
    } else {
      break;
    }
  }

  if (EndOfInput()) {
    error_message_ = "Unexpected end of input";
    error_location_ = location_;
    return false;
  }

  // Read the token value.
  const char& c = input_[index_];
  if (IsAlpha(c) || c == '_') {
    *token = GetIdentifierToken();
  } else if (IsDigit(c)) {
    *token = GetNumberToken();
  } else if (c == '\'' || c == '"') {
    *token = GetStringToken();
  } else {
    *token = GetOperatorToken();
  }

  if (*token == NULL)
    return false;

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

const Token* TokenStream::GetIdentifierToken() {
  uint start = index_++;

  while (!EndOfInput()) {
    const char& c = input_[index_];
    if (!IsAlpha(c) && !IsDigit(c) && c != '_') {
      break;
    }
    ++index_;
  }

  const Token* token = new Token(Token::IDENTIFIER,
                                 input_.substr(start, index_ - start),
                                 location_.line,
                                 location_.column);
  location_.column += index_ - start;

  return token;
}

const Token* TokenStream::GetNumberToken() {
  enum State {
    INTEGER,
    FRACTION_DIGIT,
    FRACTION,
    EXPONENT_SIGN,
    EXPONENT_DIGIT,
    EXPONENT,
    ERROR,
    END
  };

  uint start = index_++;
  State state = INTEGER;

  while (state != ERROR && state != END) {
    const char& c = EndOfInput() ? '\0' : input_[index_];
    switch (state) {
      case INTEGER:
        if (IsDigit(c)) {
          ++index_;
        } else if (c == '.') {
          state = FRACTION_DIGIT;
          ++index_;
        } else if (c == 'e' || c == 'E') {
          state = EXPONENT_SIGN;
          ++index_;
        } else {
          state = END;
        }
        break;
      case FRACTION_DIGIT:
        if (IsDigit(c)) {
          state = FRACTION;
          ++index_;
        } else {
          state = ERROR;
        }
        break;
      case FRACTION:
        if (IsDigit(c)) {
          ++index_;
        } else if (c == 'e' || c == 'E') {
          state = EXPONENT_SIGN;
          ++index_;
        } else {
          state = END;
        }
        break;
      case EXPONENT_SIGN:
        if (c == '+' || c == '-') {
          state = EXPONENT_DIGIT;
          ++index_;
        } else if (IsDigit(c)) {
          state = EXPONENT;
          ++index_;
        } else {
          state = ERROR;
        }
        break;
      case EXPONENT_DIGIT:
        if (IsDigit(c)) {
          state = EXPONENT;
          ++index_;
        } else {
          state = ERROR;
        }
        break;
      case EXPONENT:
        if (IsDigit(c)) {
          ++index_;
        } else {
          state = END;
        }
        break;
      case ERROR:
      case END:
        DASSERT(false);
        break;
    }
  }

  if (state == ERROR) {
    error_message_ = "Unterminated number";
    error_location_ = location_;
    return NULL;
  }

  const Token* token = new Token(Token::NUMBER,
                                 input_.substr(start, index_ - start),
                                 location_.line,
                                 location_.column);
  location_.column += index_ - start;

  return token;
}

const Token* TokenStream::GetStringToken() {
  enum State {
    CHARACTER,
    ESCAPE,
    END,
  };

  const char& quote = input_[index_];
  uint start = index_++;
  State state = CHARACTER;

  while (!EndOfInput() && state != END) {
    const char& c = input_[index_];
    switch (state) {
      case CHARACTER:
        if (c == '\\') {
          state = ESCAPE;
        } else if (c == quote) {
          state = END;
        }
        ++index_;
        break;
      case ESCAPE:
        state = CHARACTER;
        ++index_;
        break;
      case END:
        DASSERT(false);
        break;
    }
  }

  if (state != END) {
    error_message_ = "Unterminated string";
    error_location_ = location_;
    return NULL;
  }

  std::string value = input_.substr(start, index_ - start);
  const Token* token = new Token(Token::STRING,
                                 ReplaceEscapeCharacters(value),
                                 location_.line,
                                 location_.column);
  location_.column += index_ - start;

  return token;
}

const Token* TokenStream::GetOperatorToken() {
  return new Token(Token::OPERATOR,
                   std::string(1, input_[index_++]),
                   location_.line,
                   location_.column++);
}

std::string TokenStream::ReplaceEscapeCharacters(const std::string& str) {
  std::string ret = str;
  size_t index = ret.find('\\', 0);
  while (index != std::string::npos) {
    char c = ret[index + 1];
    if (std::string("\"'\\bnt").find(c) != std::string::npos) {
      ret.erase(index, 1);
      switch (c) {
        case 'b':
          ret[index] = '\b';
          break;
        case 'n':
          ret[index] = '\n';
          break;
        case 't':
          ret[index] = '\t';
          break;
       }
    }

    index = ret.find('\\', ++index);
  }

  return ret;
}

bool TokenStream::IsAlpha(const char& c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool TokenStream::IsDigit(const char& c) {
  return c >= '0' && c <= '9';
}
