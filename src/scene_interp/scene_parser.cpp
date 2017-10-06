#include "scene_interp/scene_parser.h"

#include <memory>
#include "scene_interp/scene_lexer.h"
#include "third_party/bonavista/src/parser/ast_node.h"

SceneParser::SceneParser(TokenStream* token_stream) : Parser(token_stream) {
}

SceneParser::~SceneParser() {
}

bool SceneParser::Parse(std::unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  std::unique_ptr<const ASTNode> node;
  if (!Parser::Parse(&node))
    return false;

  if (node.get()) {
    if (node->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS)) {
      position_ = node->token()->position();
      error_ = "Unexpected function call";
      return false;
    }

    if (!ConsumeToken(SceneLexer::TYPE_SEMI_COLON))
      return false;
  }

  *root = std::move(node);
  return true;
}

uint SceneParser::GetBindingPower(int type) const {
  switch (type) {
    case SceneLexer::TYPE_DOT:
      return 20;
    case SceneLexer::TYPE_EQUAL:
      return 10;
    case SceneLexer::TYPE_LEFT_PARENTHESIS:
      return 30;
    default:
      return 0;
  }
}

bool SceneParser::ParsePrefixToken(std::unique_ptr<const Token> token,
                                   std::unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  if (token->IsType(SceneLexer::TYPE_IDENTIFIER) ||
      token->IsType(SceneLexer::TYPE_NUMBER)) {
    root->reset(new ASTNode(std::move(token)));
    return true;
  }

  if (token->IsType(SceneLexer::TYPE_NEW))
    return ParseNewObject(std::move(token), root);

  position_ = token->position();
  error_ = StringFormat("Unexpected token: %s", token->value().c_str());
  return false;
}

bool SceneParser::ParseInfixToken(std::unique_ptr<const Token> token,
                                  std::unique_ptr<const ASTNode> left,
                                  std::unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  if (token->IsType(SceneLexer::TYPE_DOT))
    return ParseDotAccessor(std::move(token), std::move(left), root);

  if (token->IsType(SceneLexer::TYPE_EQUAL))
    return ParseAssignment(std::move(token), std::move(left), root);

  if (token->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS))
    return ParseFunction(std::move(token), std::move(left), root);

  position_ = token->position();
  error_ = StringFormat("Unexpected token: %s", token->value().c_str());
  return false;
}

bool SceneParser::ParseNewObject(std::unique_ptr<const Token> token,
                                 std::unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  std::unique_ptr<const ASTNode> right;
  if (!ParseExpression(0, &right))
    return false;
  if (!right->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS)) {
    position_ = right->token()->position();
    error_ = "Expecting function call on right of new";
    return false;
  }
  node->AddChild(std::move(right));

  *root = std::move(node);
  return true;
}

bool SceneParser::ParseDotAccessor(std::unique_ptr<const Token> token,
                                   std::unique_ptr<const ASTNode> left,
                                   std::unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!left->token()->IsType(SceneLexer::TYPE_IDENTIFIER)) {
    position_ = left->token()->position();
    error_ = "Expecting identifier on left of dot accessor";
    return false;
  }
  node->AddChild(std::move(left));

  std::unique_ptr<const ASTNode> right;
  if (!ParseExpression(GetBindingPower(node->token()->type()), &right))
    return false;
  if (!right->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS)) {
    position_ = right->token()->position();
    error_ = "Expecting function call on right of dot accessor";
    return false;
  }
  node->AddChild(std::move(right));

  *root = std::move(node);
  return true;
}

bool SceneParser::ParseAssignment(std::unique_ptr<const Token> token,
                                  std::unique_ptr<const ASTNode> left,
                                  std::unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!left->token()->IsType(SceneLexer::TYPE_IDENTIFIER)) {
    position_ = left->token()->position();
    error_ = "Expecting identifier on left of assignment";
    return false;
  }
  node->AddChild(std::move(left));

  std::unique_ptr<const ASTNode> right;
  if (!ParseExpression(GetBindingPower(node->token()->type()), &right))
    return false;
  node->AddChild(std::move(right));

  *root = std::move(node);
  return true;
}

bool SceneParser::ParseFunction(std::unique_ptr<const Token> token,
                                std::unique_ptr<const ASTNode> left,
                                std::unique_ptr<const ASTNode>* root) {
  DCHECK(root);

  std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!left->token()->IsType(SceneLexer::TYPE_IDENTIFIER)) {
    position_ = left->token()->position();
    error_ = "Expecting identifier on left of function call";
    return false;
  }
  node->AddChild(std::move(left));

  if (!look_ahead_token_->IsType(SceneLexer::TYPE_RIGHT_PARENTHESIS)) {
    while (true) {
      std::unique_ptr<const ASTNode> arg;
      if (!ParseExpression(0, &arg))
        return false;
      node->AddChild(std::move(arg));

      if (!look_ahead_token_->IsType(SceneLexer::TYPE_COMMA))
        break;

      if (!ConsumeToken(SceneLexer::TYPE_COMMA))
        return false;
    }
  }

  if (!ConsumeToken(SceneLexer::TYPE_RIGHT_PARENTHESIS))
    return false;

  *root = std::move(node);
  return true;
}
