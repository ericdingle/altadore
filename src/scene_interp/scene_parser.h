#ifndef SCENE_INTERP_SCENE_PARSER_H_
#define SCENE_INTERP_SCENE_PARSER_H_

#include "third_party/bonavista/src/parser/parser.h"

class SceneParser : public Parser {
 public:
  explicit SceneParser(TokenStream* token_stream);
  SceneParser(const SceneParser&) = delete;
  SceneParser& operator=(const SceneParser&) = delete;
  ~SceneParser() override = default;

  StatusOr<std::unique_ptr<Node>> Parse() override;

 protected:
  int GetBindingPower(int type) const override;
  StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token>) override;
  StatusOr<std::unique_ptr<Node>> ParseInfixToken(
      std::unique_ptr<const Token>, std::unique_ptr<const Node> left) override;

 private:
  StatusOr<std::unique_ptr<Node>> ParseDotAccessor(
      std::unique_ptr<const Token> token, std::unique_ptr<const Node> left);
  StatusOr<std::unique_ptr<Node>> ParseAssignment(
      std::unique_ptr<const Token> token, std::unique_ptr<const Node> left);
  StatusOr<std::unique_ptr<Node>> ParseFunction(
      std::unique_ptr<const Token> token, std::unique_ptr<const Node> left);
};

#endif  // SCENE_INTERP_SCENE_PARSER_H_
