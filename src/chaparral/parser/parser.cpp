#include "chaparral/parser/parser.h"

#include "bonavista/logging/string_format.h"
#include "chaparral/lexer/lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

Parser::Parser(TokenStream* token_stream) : token_stream_(token_stream) {
  DCHECK(token_stream);
}

Parser::~Parser() {
}

bool Parser::Parse(const ASTNode** root) {
  DCHECK(root);

  if (!Init())
    return false;

  // Check for end of input.
  if (look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT)) {
    *root = NULL;
    return true;
  }

  // Parse expression.
  scoped_ptr<const ASTNode> node;
  if (!ParseExpression(0, node.Receive()))
    return false;

  *root = node.Release();
  return true;
}

bool Parser::HasInput() const {
  return look_ahead_token_.ptr() ?
      !look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT) :
      token_stream_->HasInput();
}

const Token::Position& Parser::position() const {
  return position_;
}

const std::string& Parser::error() const {
  return error_;
}

bool Parser::Init() {
  if (look_ahead_token_.ptr())
    return true;

  // Advance the look ahead token to the first token.
  scoped_ptr<const Token> token;
  return GetNextToken(token.Receive());
}

bool Parser::GetNextToken(const Token** token) {
  DCHECK(token);

  *token = look_ahead_token_.Release();

  if (!token_stream_->GetNextToken(look_ahead_token_.Receive())) {
    position_ = token_stream_->position();
    error_ = token_stream_->error();
    return false;
  }
  DCHECK(look_ahead_token_.ptr());

  return true;
}

bool Parser::ParseExpression(uint binding_power, const ASTNode** root) {
  scoped_ptr<const Token> token;
  if (!GetNextToken(token.Receive()))
    return false;

  scoped_ptr<const ASTNode> left;
  if (!ParsePrefixToken(token.Release(), left.Receive()))
    return false;
  DCHECK(left.ptr());

  while (binding_power < GetBindingPower(look_ahead_token_->type())) {
    if (!GetNextToken(token.Receive()))
      return false;

    scoped_ptr<const ASTNode> node;
    if (!ParseInfixToken(token.Release(), left.Release(), node.Receive()))
      return false;
    DCHECK(node.ptr());

    left.Reset(node.Release());
  }

  *root = left.Release();
  return true;
}

bool Parser::ConsumeToken(int type) {
  scoped_ptr<const Token> token;
  if (!GetNextToken(token.Receive()))
    return false;

  if (!token->IsType(type)) {
    position_ = token->position();
    error_ = StringFormat("Unexpected token: %s", token->value().c_str());
    return false;
  }

  return true;
}

uint Parser::GetBindingPower(int type) const {
  return 0;
}

bool Parser::ParseInfixToken(const Token* token, const ASTNode* left,
                             const ASTNode** root) {
  DCHECK(token);

  scoped_ptr<const Token> token_deleter(token);
  scoped_ptr<const ASTNode> left_deleter(left);

  position_ = token->position();
  error_ = StringFormat("Unexpected token: %s", token->value().c_str());
  return false;
}
