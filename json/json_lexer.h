#ifndef CHAPARRAL_JSON_JSON_LEXER_H_
#define CHAPARRAL_JSON_JSON_LEXER_H_

#include "bonavista/base/macros.h"
#include "chaparral/lexer/lexer.h"

class JsonLexer : public Lexer {
 public:
  JsonLexer();
  virtual ~JsonLexer();

  enum Type {
    TYPE_COLON,
    TYPE_COMMA,
    TYPE_FALSE,
    TYPE_LEFT_BRACE,
    TYPE_LEFT_BRACKET,
    TYPE_NULL,
    TYPE_NUMBER,
    TYPE_RIGHT_BRACE,
    TYPE_RIGHT_BRACKET,
    TYPE_TRUE,
    TYPE_STRING
  };

  virtual bool GetToken(const std::string& input,
                        uint index,
                        int* type,
                        std::string* value,
                        uint* count,
                        std::string* error) const;

 private:
  bool GetKeywordToken(const std::string& input,
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

  bool GetStringToken(const std::string& input,
                      uint index,
                      int* type,
                      std::string* value,
                      uint* count,
                      std::string* error) const;


  DISALLOW_COPY_AND_ASSIGN(JsonLexer);
};

#endif
