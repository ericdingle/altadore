#ifndef CHAPARRAL_CALC_CALC_PARSER_H_
#define CHAPARRAL_CALC_CALC_PARSER_H_

#include "chaparral/parser/parser.h"

class CalcParser : public Parser {
 public:
  CalcParser(TokenStream* token_stream);
  virtual ~CalcParser();

 protected:
  virtual uint GetBindingPower(int type) const;
  virtual bool ParsePrefixToken(const Token* token, const ASTNode** root);
  virtual bool ParseInfixToken(const Token* token, const ASTNode* left,
                               const ASTNode** root);

 private:
  DISALLOW_COPY_AND_ASSIGN(CalcParser);
};

#endif
