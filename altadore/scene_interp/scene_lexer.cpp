#include "altadore/scene_interp/scene_lexer.h"

#include "bonavista/util/status_macros.h"

absl::StatusOr<std::unique_ptr<Token>> SceneLexer::GetToken(
    std::string_view input, int line, int column) const {
  char c = input[0];

  if (IsAlpha(c)) {
    return GetIdentifierToken(input.data(), line, column);
  } else if (c == '-' || IsDigit(c)) {
    return GetNumberToken(input.data(), line, column);
  }

  int t = -1;
  if (c == ',') {
    t = TYPE_COMMA;
  } else if (c == '.') {
    t = TYPE_DOT;
  } else if (c == '=') {
    t = TYPE_EQUAL;
  } else if (c == '(') {
    t = TYPE_LEFT_PARENTHESIS;
  } else if (c == ')') {
    t = TYPE_RIGHT_PARENTHESIS;
  } else if (c == ';') {
    t = TYPE_SEMI_COLON;
  }

  if (t != -1) {
    return std::make_unique<Token>(t, input.substr(0, 1), line, column);
  }

  return UnexpectedCharacterError(c, line, column);
}

absl::StatusOr<std::unique_ptr<Token>> SceneLexer::GetIdentifierToken(
    const char* input, int line, int column) const {
  const char* start = input;
  ++input;

  for (; IsAlpha(*input) || IsDigit(*input) || *input == '_'; ++input);

  return std::make_unique<Token>(
      TYPE_IDENTIFIER, std::string_view(start, input - start), line, column);
}

absl::StatusOr<std::unique_ptr<Token>> SceneLexer::GetNumberToken(
    const char* input, int line, int column) const {
  const char* start = input;

  if (*input == '-') {
    ++input;
  }

  RETURN_IF_ERROR(ExpectDigit(*input, line, column));
  if (*input == '0') {
    ++input;
  } else {
    for (; IsDigit(*input); ++input);
  }

  if (*input == '.') {
    ++input;
    RETURN_IF_ERROR(ExpectDigit(*input, line, column));
    for (; IsDigit(*input); ++input);
  }

  return std::make_unique<Token>(
      TYPE_NUMBER, std::string_view(start, input - start), line, column);
}
