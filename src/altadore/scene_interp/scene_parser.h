#ifndef ALTADORE_SCENE_INTERP_SCENE_PARSER_H_
#define ALTADORE_SCENE_INTERP_SCENE_PARSER_H_

#include "bonavista/util/macros.h"
#include "chaparral/parser/parser.h"

class SceneParser : public Parser {
 public:
  SceneParser(TokenStream* token_stream);
  virtual ~SceneParser();

  virtual bool Parse(const ASTNode** root);

 protected:
  virtual uint GetBindingPower(int type) const;
  virtual bool ParsePrefixToken(const Token* token, const ASTNode** root);
  virtual bool ParseInfixToken(const Token* token, const ASTNode* left,
                               const ASTNode** root);

 private:
  bool ParseNewObject(const Token* token, const ASTNode** root);
  bool ParseDotAccessor(const Token* token, const ASTNode* left,
                        const ASTNode** root);
  bool ParseAssignment(const Token* token, const ASTNode* left,
                       const ASTNode** root);
  bool ParseFunction(const Token* token, const ASTNode* left,
                     const ASTNode** root);

  DISALLOW_COPY_AND_ASSIGN(SceneParser);
};

#endif
