#ifndef CHAPARRAL_JSON_JSON_PARSER_H_
#define CHAPARRAL_JSON_JSON_PARSER_H_

#include "chaparral/parser/parser.h"

class JsonParser : public Parser {
 public:
  JsonParser();
  virtual ~JsonParser();

  bool Parse(TokenStream* token_stream, const ASTNode** root);

 protected:
  using Parser::Parse;

  virtual uint GetBindingPower(int type) const;
  virtual bool ParsePrefixToken(const Token* token, const ASTNode** root);
  virtual bool ParseInfixToken(const Token* token, const ASTNode* left,
                               const ASTNode** root);

 private:
  bool ParseObject(const Token* token, const ASTNode** root);
  bool ParseArray(const Token* token, const ASTNode** root);

  DISALLOW_COPY_AND_ASSIGN(JsonParser);
};

#endif
