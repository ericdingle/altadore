#include "altadore/scene_interp/scene_parser.h"

#include "altadore/scene_interp/scene_lexer.h"
#include "bonavista/logging/string_format.h"
#include "bonavista/memory/scoped_ptr.h"
#include "chaparral/parser/ast_node.h"

SceneParser::SceneParser(TokenStream* token_stream) : Parser(token_stream) {
}

SceneParser::~SceneParser() {
}

bool SceneParser::Parse(const ASTNode** root) {
  DCHECK(root);

  scoped_ptr<const ASTNode> node;
  if (!Parser::Parse(node.Receive()))
    return false;

  if (node.ptr()) {
    if (node->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS)) {
      position_ = node->token()->position();
      error_ = "Unexpected function call";
      return false;
    }

    if (!ConsumeToken(SceneLexer::TYPE_SEMI_COLON))
      return false;
  }

  *root = node.Release();
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

bool SceneParser::ParsePrefixToken(const Token* token, const ASTNode** root) {
  DCHECK(token);
  DCHECK(root);

  scoped_ptr<const Token> token_holder(token);

  if (token->IsType(SceneLexer::TYPE_IDENTIFIER) ||
      token->IsType(SceneLexer::TYPE_NUMBER)) {
    *root = new ASTNode(token_holder.Release());
    return true;
  }

  if (token->IsType(SceneLexer::TYPE_NEW))
    return ParseNewObject(token_holder.Release(), root);

  position_ = token->position();
  error_ = StringFormat("Unexpected token: %s", token->value().c_str());
  return false;
}

bool SceneParser::ParseInfixToken(const Token* token, const ASTNode* left,
                                  const ASTNode** root) {
  DCHECK(token);
  DCHECK(left);
  DCHECK(root);

  scoped_ptr<const Token> token_holder(token);
  scoped_ptr<const ASTNode> left_holder(left);

  if (token->IsType(SceneLexer::TYPE_DOT))
    return ParseDotAccessor(token_holder.Release(), left_holder.Release(), root);

  if (token->IsType(SceneLexer::TYPE_EQUAL))
    return ParseAssignment(token_holder.Release(), left_holder.Release(), root);

  if (token->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS))
    return ParseFunction(token_holder.Release(), left_holder.Release(), root);

  position_ = token->position();
  error_ = StringFormat("Unexpected token: %s", token->value().c_str());
  return false;
}

bool SceneParser::ParseNewObject(const Token* token, const ASTNode** root) {
  DCHECK(token);
  DCHECK(root);

  scoped_ptr<ASTNode> node(new ASTNode(token));

  scoped_ptr<const ASTNode> right;
  if (!ParseExpression(0, right.Receive()))
    return false;
  if (!right->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS)) {
    position_ = right->token()->position();
    error_ = "Expecting function call on right of new";
    return false;
  }
  node->AddChild(right.Release());

  *root = node.Release();
  return true;
}

bool SceneParser::ParseDotAccessor(const Token* token, const ASTNode* left,
                                   const ASTNode** root) {
  DCHECK(token);
  DCHECK(left);
  DCHECK(root);

  scoped_ptr<const ASTNode> left_holder(left);

  scoped_ptr<ASTNode> node(new ASTNode(token));

  if (!left->token()->IsType(SceneLexer::TYPE_IDENTIFIER)) {
    position_ = left->token()->position();
    error_ = "Expecting identifier on left of dot accessor";
    return false;
  }
  node->AddChild(left_holder.Release());

  scoped_ptr<const ASTNode> right;
  if (!ParseExpression(GetBindingPower(token->type()), right.Receive()))
    return false;
  if (!right->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS)) {
    position_ = right->token()->position();
    error_ = "Expecting function call on right of dot accessor";
    return false;
  }
  node->AddChild(right.Release());

  *root = node.Release();
  return true;
}

bool SceneParser::ParseAssignment(const Token* token, const ASTNode* left,
                                  const ASTNode** root) {
  DCHECK(token);
  DCHECK(left);
  DCHECK(root);

  scoped_ptr<const ASTNode> left_holder(left);

  scoped_ptr<ASTNode> node(new ASTNode(token));

  if (!left->token()->IsType(SceneLexer::TYPE_IDENTIFIER)) {
    position_ = left->token()->position();
    error_ = "Expecting identifier on left of assignment";
    return false;
  }
  node->AddChild(left_holder.Release());

  scoped_ptr<const ASTNode> right;
  if (!ParseExpression(GetBindingPower(token->type()), right.Receive()))
    return false;
  node->AddChild(right.Release());

  *root = node.Release();
  return true;
}

bool SceneParser::ParseFunction(const Token* token, const ASTNode* left,
                                const ASTNode** root) {
  DCHECK(token);
  DCHECK(left);
  DCHECK(root);

  scoped_ptr<const ASTNode> left_holder(left);

  scoped_ptr<ASTNode> node(new ASTNode(token));

  if (!left->token()->IsType(SceneLexer::TYPE_IDENTIFIER)) {
    position_ = left->token()->position();
    error_ = "Expecting identifier on left of function call";
    return false;
  }
  node->AddChild(left_holder.Release());

  if (!look_ahead_token_->IsType(SceneLexer::TYPE_RIGHT_PARENTHESIS)) {
    while (true) {
      scoped_ptr<const ASTNode> arg;
      if (!ParseExpression(0, arg.Receive()))
        return false;
      node->AddChild(arg.Release());

      if (!look_ahead_token_->IsType(SceneLexer::TYPE_COMMA))
        break;

      if (!ConsumeToken(SceneLexer::TYPE_COMMA))
        return false;
    }
  }

  if (!ConsumeToken(SceneLexer::TYPE_RIGHT_PARENTHESIS))
    return false;

  *root = node.Release();
  return true;
}
