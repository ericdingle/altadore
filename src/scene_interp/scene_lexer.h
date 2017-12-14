#ifndef SCENE_INTERP_SCENE_LEXER_H_
#define SCENE_INTERP_SCENE_LEXER_H_

#include "third_party/bonavista/src/lexer/lexer.h"

class SceneLexer : public Lexer {
 public:
  SceneLexer() = default;
  SceneLexer(const SceneLexer&) = delete;
  SceneLexer& operator=(const SceneLexer&) = delete;
  ~SceneLexer() override = default;

  enum Type {
    TYPE_COMMA,
    TYPE_DOT,
    TYPE_EQUAL,
    TYPE_IDENTIFIER,
    TYPE_LEFT_PARENTHESIS,
    TYPE_NUMBER,
    TYPE_RIGHT_PARENTHESIS,
    TYPE_SEMI_COLON
  };

  StatusOr<std::unique_ptr<Token>> GetToken(
      const char* input, int line, int column) const override;

 private:
  StatusOr<std::unique_ptr<Token>> GetIdentifierToken(
      const char* input, int line, int column) const;
  StatusOr<std::unique_ptr<Token>> GetNumberToken(
      const char* input, int line, int column) const;
};

#endif  // SCENE_INTERP_SCENE_LEXER_H_
