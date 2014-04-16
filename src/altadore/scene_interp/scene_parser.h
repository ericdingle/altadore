#ifndef ALTADORE_SCENE_INTERP_SCENE_PARSER_H_
#define ALTADORE_SCENE_INTERP_SCENE_PARSER_H_

#include "bonavista/base/macros.h"
#include "chaparral/parser/parser.h"

class SceneParser : public Parser {
 public:
  SceneParser(TokenStream* token_stream);
  virtual ~SceneParser();

  virtual bool Parse(std::unique_ptr<const ASTNode>* root);

 protected:
  virtual uint GetBindingPower(int type) const;
  virtual bool ParsePrefixToken(std::unique_ptr<const Token> token,
                                std::unique_ptr<const ASTNode>* root);
  virtual bool ParseInfixToken(std::unique_ptr<const Token> token,
                               std::unique_ptr<const ASTNode> left,
                               std::unique_ptr<const ASTNode>* root);

 private:
  bool ParseNewObject(std::unique_ptr<const Token> token,
                      std::unique_ptr<const ASTNode>* root);
  bool ParseDotAccessor(std::unique_ptr<const Token> token,
                        std::unique_ptr<const ASTNode> left,
                        std::unique_ptr<const ASTNode>* root);
  bool ParseAssignment(std::unique_ptr<const Token> token,
                       std::unique_ptr<const ASTNode> left,
                       std::unique_ptr<const ASTNode>* root);
  bool ParseFunction(std::unique_ptr<const Token> token,
                     std::unique_ptr<const ASTNode> left,
                     std::unique_ptr<const ASTNode>* root);

  DISALLOW_COPY_AND_ASSIGN(SceneParser);
};

#endif
