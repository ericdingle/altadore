#include "scene_interp/scene_lexer.h"

StatusOr<std::unique_ptr<Token>> SceneLexer::GetToken(
    const char* input, int line, int column) const {
  char c = *input;

  if (IsAlpha(c)) {
    return GetIdentifierToken(input, line, column);
  } else if (c == '-' || IsDigit(c)) {
    return GetNumberToken(input, line, column);
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
    return std::unique_ptr<Token>(new Token(t, c, line, column));
  }

  return UnexpectedCharacter(c, line, column);
}

StatusOr<std::unique_ptr<Token>> SceneLexer::GetIdentifierToken(
    const char* input, int line, int column) const {
  const char* start = input;
  ++input;

  for (; IsAlpha(*input) || IsDigit(*input) || *input == '_'; ++input);

  return std::unique_ptr<Token>(new Token(
      TYPE_IDENTIFIER, std::string(start, input - start), line, column));
}

StatusOr<std::unique_ptr<Token>> SceneLexer::GetNumberToken(
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

  return std::unique_ptr<Token>(new Token(
      TYPE_NUMBER, std::string(start, input - start), line, column));
}
