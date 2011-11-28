#ifndef CHAPARRAL_LEXER_LEXER_H_
#define CHAPARRAL_LEXER_LEXER_H_

#include <string>
#include "bonavista/base/macros.h"

class Token;

class Lexer {
 public:
  Lexer();
  virtual ~Lexer();

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const =0;

 protected:
  static bool IsAlpha(const char& c);
  static bool IsDigit(const char& c);

 private:
  DISALLOW_COPY_AND_ASSIGN(Lexer);
};

#endif
