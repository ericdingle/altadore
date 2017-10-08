#ifndef SCENE_INTERP_SCENE_LEXER_H_
#define SCENE_INTERP_SCENE_LEXER_H_

#include "third_party/bonavista/src/lexer/lexer.h"

class SceneLexer : public Lexer {
 public:
  SceneLexer();
  SceneLexer(const SceneLexer&) = delete;
  SceneLexer& operator=(const SceneLexer&) = delete;
  virtual ~SceneLexer();

  enum Type {
    TYPE_COMMA,
    TYPE_DOT,
    TYPE_EQUAL,
    TYPE_IDENTIFIER,
    TYPE_LEFT_PARENTHESIS,
    TYPE_NEW,
    TYPE_NUMBER,
    TYPE_RIGHT_PARENTHESIS,
    TYPE_SEMI_COLON
  };

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const;

 private:
  bool GetIdentifierToken(const std::string& input,
                          int index,
                          int* type,
                          std::string* value,
                          int* count,
                          std::string* error) const;

  bool GetNumberToken(const std::string& input,
                      int index,
                      int* type,
                      std::string* value,
                      int* count,
                      std::string* error) const;
};

#endif  // SCENE_INTERP_SCENE_LEXER_H_
