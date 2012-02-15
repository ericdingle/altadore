#ifndef ALTADORE_SCENE_INTERP_SCENE_LEXER_H_
#define ALTADORE_SCENE_INTERP_SCENE_LEXER_H_

#include "bonavista/util/macros.h"
#include "chaparral/lexer/lexer.h"

class SceneLexer : public Lexer {
 public:
  SceneLexer();
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
                        uint index,
                        int* type,
                        std::string* value,
                        uint* count,
                        std::string* error) const;

 private:
  bool GetIdentifierToken(const std::string& input,
                          uint index,
                          int* type,
                          std::string* value,
                          uint* count,
                          std::string* error) const;

  bool GetNumberToken(const std::string& input,
                      uint index,
                      int* type,
                      std::string* value,
                      uint* count,
                      std::string* error) const;

  DISALLOW_COPY_AND_ASSIGN(SceneLexer);
};

#endif
