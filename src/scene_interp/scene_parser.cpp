#include "scene_interp/scene_parser.h"

#include <memory>
#include "scene_interp/scene_lexer.h"

SceneParser::SceneParser(TokenStream* token_stream) : Parser(token_stream) {
}


StatusOr<std::unique_ptr<Node>> SceneParser::Parse() {
  ASSIGN_OR_RETURN(auto node, Parser::Parse());
  RETURN_IF_ERROR(ConsumeToken(SceneLexer::TYPE_SEMI_COLON));
  return std::move(node);
}

int SceneParser::GetBindingPower(int type) const {
  switch (type) {
    case SceneLexer::TYPE_DOT:
      return 30;
    case SceneLexer::TYPE_EQUAL:
      return 10;
    case SceneLexer::TYPE_LEFT_PARENTHESIS:
      return 20;
    default:
      return 0;
  }
}

StatusOr<std::unique_ptr<Node>> SceneParser::ParsePrefixToken(
    std::unique_ptr<const Token> token) {
  if (token->IsType(SceneLexer::TYPE_IDENTIFIER) ||
      token->IsType(SceneLexer::TYPE_NUMBER)) {
    return std::unique_ptr<Node>(new Node(std::move(token)));
  }

  return UnexpectedToken(*token);
}

StatusOr<std::unique_ptr<Node>> SceneParser::ParseInfixToken(
    std::unique_ptr<const Token> token, std::unique_ptr<const Node> left) {
  if (token->IsType(SceneLexer::TYPE_DOT))
    return ParseDotAccessor(std::move(token), std::move(left));

  if (token->IsType(SceneLexer::TYPE_EQUAL))
    return ParseAssignment(std::move(token), std::move(left));

  if (token->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS))
    return ParseFunction(std::move(token), std::move(left));


  return UnexpectedToken(*token);
}

StatusOr<std::unique_ptr<Node>> SceneParser::ParseDotAccessor(
    std::unique_ptr<const Token> token, std::unique_ptr<const Node> left) {
  std::unique_ptr<Node> node(new Node(std::move(token)));

  node->AddChild(std::move(left));

  ASSIGN_OR_RETURN(auto right, ParseExpression(GetBindingPower(node->token().type())));
  RETURN_IF_ERROR(ExpectToken(right->token(), SceneLexer::TYPE_IDENTIFIER));
  node->AddChild(std::move(right));

  return std::move(node);
}

StatusOr<std::unique_ptr<Node>> SceneParser::ParseAssignment(
    std::unique_ptr<const Token> token, std::unique_ptr<const Node> left) {
  std::unique_ptr<Node> node(new Node(std::move(token)));

  RETURN_IF_ERROR(ExpectToken(left->token(), SceneLexer::TYPE_IDENTIFIER));
  node->AddChild(std::move(left));

  ASSIGN_OR_RETURN(auto right, ParseExpression(GetBindingPower(node->token().type())));
  node->AddChild(std::move(right));

  return std::move(node);
}

StatusOr<std::unique_ptr<Node>> SceneParser::ParseFunction(
    std::unique_ptr<const Token> token, std::unique_ptr<const Node> left) {
  std::unique_ptr<Node> node(new Node(std::move(token)));

  node->AddChild(std::move(left));

  if (!look_ahead_token_->IsType(SceneLexer::TYPE_RIGHT_PARENTHESIS)) {
    while (true) {
      ASSIGN_OR_RETURN(auto arg, ParseExpression(0));
      node->AddChild(std::move(arg));

      if (!look_ahead_token_->IsType(SceneLexer::TYPE_COMMA))
        break;
      RETURN_IF_ERROR(ConsumeToken(SceneLexer::TYPE_COMMA));
    }
  }

  RETURN_IF_ERROR(ConsumeToken(SceneLexer::TYPE_RIGHT_PARENTHESIS));

  return std::move(node);
}
