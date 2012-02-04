#ifndef CHAPARRAL_CALC_CALC_LEXER_H_
#define CHAPARRAL_CALC_CALC_LEXER_H_

#include "bonavista/util/macros.h"
#include "chaparral/lexer/lexer.h"

class CalcLexer : public Lexer {
 public:
  CalcLexer();
  ~CalcLexer();

  enum Type {
    TYPE_ASTERISK,
    TYPE_LEFT_PARENTHESIS,
    TYPE_MINUS,
    TYPE_NUMBER,
    TYPE_PLUS,
    TYPE_RIGHT_PARENTHESIS,
    TYPE_SLASH
  };

  virtual bool GetToken(const std::string& input,
                        uint index,
                        int* type,
                        std::string* value,
                        uint* count,
                        std::string* error) const;

 private:
  bool GetNumberToken(const std::string& input,
                      uint index,
                      int* type,
                      std::string* value,
                      uint* count,
                      std::string* error) const;

  DISALLOW_COPY_AND_ASSIGN(CalcLexer);
};

#endif
