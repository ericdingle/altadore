#include "chaparral/calc/calc_parser.h"

#include <vector>
#include "bonavista/string/format.h"
#include "chaparral/calc/calc_lexer.h"
#include "chaparral/parser/ast_node.h"

CalcParser::CalcParser(TokenStream* token_stream) : Parser(token_stream) {
}

CalcParser::~CalcParser() {
}

uint CalcParser::GetBindingPower(int type) const {
  switch (type) {
    case CalcLexer::TYPE_ASTERISK:
    case CalcLexer::TYPE_SLASH:
      return 20;
    case CalcLexer::TYPE_MINUS:
    case CalcLexer::TYPE_PLUS:
      return 10;
    default:
      return 0;
  }
}

bool CalcParser::ParsePrefixToken(const Token* token, const ASTNode** root) {
  ASSERT(token);
  ASSERT(root);

  memory::scoped_ptr<const Token> token_holder(token);

  if (token->IsType(CalcLexer::TYPE_LEFT_PARENTHESIS)) {
    memory::scoped_ptr<const ASTNode> node;
    if (!ParseExpression(0, node.Receive()))
      return false;

    if (!ConsumeToken(CalcLexer::TYPE_RIGHT_PARENTHESIS))
      return false;

    *root = node.Release();
    return true;
  }

  if (token->IsType(CalcLexer::TYPE_NUMBER)) {
    *root = new ASTNode(token_holder.Release());
    return true;
  }

  position_ = token->position();
  error_ = string::Format("Unexpected token: %s", token->value().c_str());
  return false;
}

bool CalcParser::ParseInfixToken(const Token* token, const ASTNode* left,
                                 const ASTNode** root) {
  ASSERT(token);
  ASSERT(left);
  ASSERT(root);

  memory::scoped_ptr<const Token> token_holder(token);
  memory::scoped_ptr<const ASTNode> left_holder(left);

  if (token->IsType(CalcLexer::TYPE_ASTERISK) ||
      token->IsType(CalcLexer::TYPE_MINUS) ||
      token->IsType(CalcLexer::TYPE_PLUS) ||
      token->IsType(CalcLexer::TYPE_SLASH)) {
    memory::scoped_ptr<ASTNode> node(new ASTNode(token_holder.Release()));
    node->AddChild(left_holder.Release());

    memory::scoped_ptr<const ASTNode> right;
    if (!ParseExpression(GetBindingPower(token->type()), right.Receive()))
      return false;
    node->AddChild(right.Release());

    *root = node.Release();
    return true;
  }

  position_ = token->position();
  error_ = string::Format("Unexpected token: %s", token->value().c_str());
  return false;
}
