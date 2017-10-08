#ifndef SCENE_INTERP_SCENE_PARSER_H_
#define SCENE_INTERP_SCENE_PARSER_H_

#include "third_party/bonavista/src/parser/parser.h"

class SceneParser : public Parser {
 public:
  explicit SceneParser(TokenStream* token_stream);
  SceneParser(const SceneParser&) = delete;
  SceneParser& operator=(const SceneParser&) = delete;
  virtual ~SceneParser();

  virtual bool Parse(std::unique_ptr<const ASTNode>* root);

 protected:
  virtual int GetBindingPower(int type) const;
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
};

#endif  // SCENE_INTERP_SCENE_PARSER_H_
