#include "altadore/scene_interp/scene_lexer.h"

#include "bonavista/string/format.h"

SceneLexer::SceneLexer() {
}

SceneLexer::~SceneLexer() {
}

bool SceneLexer::GetToken(const std::string& input,
                          uint index,
                          int* type,
                          std::string* value,
                          uint* count,
                          std::string* error) const {
  const char& c = input[index];

  if (IsAlpha(c))
    return GetIdentifierToken(input, index, type, value, count, error);
  if (IsDigit(c))
    return GetNumberToken(input, index, type, value, count, error);

  int t = -1;
  if (c == ',')
    t = TYPE_COMMA;
  else if (c == '.')
    t = TYPE_DOT;
  else if (c == '=')
    t = TYPE_EQUAL;
  else if (c == '(')
    t = TYPE_LEFT_PARENTHESIS;
  else if (c == ')')
    t = TYPE_RIGHT_PARENTHESIS;
  else if (c == ';')
    t = TYPE_SEMI_COLON;

  if (t != -1) {
    *type = t;
    *value = c;
    *count = 1;
    return true;
  }

  *error = string::Format("Unrecognized token: %c", c);
  return false;
}

bool SceneLexer::GetIdentifierToken(const std::string& input,
                                    uint index,
                                    int* type,
                                    std::string* value,
                                    uint* count,
                                    std::string* error) const {
  const uint length = input.length();
  const uint start = index;

  ++index;
  for (; index < length && (IsAlpha(input[index]) || IsDigit(input[index]) || input[index] == '_'); ++index);

  *count = index - start;
  *value = input.substr(start, *count);
  if (*value == "new")
    *type = TYPE_NEW;
  else
    *type = TYPE_IDENTIFIER;
  return true;
}

bool SceneLexer::GetNumberToken(const std::string& input,
                                uint index,
                                int* type,
                                std::string* value,
                                uint* count,
                                std::string* error) const {
  const uint length = input.length();
  const uint start = index;

  if (input[index] == '0') {
    ++index;
    if (index < length && IsDigit(input[index])) {
      *error = "Unexpected digit";
      return false;
    }
  } else
    for (; index < length && IsDigit(input[index]); ++index);

  if (index < length && input[index] == '.') {
    ++index;

    if (index == length) {
      *error = "Unexpected end of input";
      return false;
    } else if (!IsDigit(input[index])) {
      *error = "Expecting digit";
      return false;
    }

    for (; index < length && IsDigit(input[index]); ++index);
  }

  *type = TYPE_NUMBER;
  *value = input.substr(start, index - start);
  *count = index - start;
  return true;
}
