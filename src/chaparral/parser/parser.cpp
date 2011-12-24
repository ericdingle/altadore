#include "chaparral/parser/parser.h"

#include "bonavista/logging/assert.h"
#include "bonavista/string/format.h"
#include "chaparral/lexer/lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

Parser::Parser() {
}

Parser::~Parser() {
}

bool Parser::Parse(TokenStream* token_stream,
                   std::vector<const ASTNode*>* nodes) {
  ASSERT(token_stream);
  ASSERT(!token_stream_);

  position_ = Token::Position();
  error_.clear();
  token_stream_ = token_stream;

  bool result = Parse(nodes);

  look_ahead_token_.Reset(NULL);
  token_stream_ = NULL;

  return result;
}

const Token::Position& Parser::position() const {
  return position_;
}

const std::string& Parser::error() const {
  return error_;
}

bool Parser::Parse(std::vector<const ASTNode*>* nodes) {
  ASSERT(nodes);
  nodes->clear();

  // Advance look ahead to the first token.
  memory::scoped_ptr<const Token> token;
  if (!GetNextToken(token.Receive()))
    return false;
  DASSERT(!token.ptr());

  // Parse expressions.
  while (!look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT)) {
    memory::scoped_ptr<const ASTNode> node;
    if (!ParseExpression(0, node.Receive()))
      return false;
    nodes->push_back(node.Release());
  }

  return true;
}

bool Parser::GetNextToken(const Token** token) {
  ASSERT(token);

  *token = look_ahead_token_.Release();

  if (!token_stream_->GetNextToken(look_ahead_token_.Receive())) {
    position_ = token_stream_->position();
    error_ = token_stream_->error();
    return false;
  }
  DASSERT(look_ahead_token_.ptr());

  return true;
}

bool Parser::ParseExpression(uint binding_power, const ASTNode** root) {
  memory::scoped_ptr<const Token> token;
  if (!GetNextToken(token.Receive()))
    return false;

  memory::scoped_ptr<const ASTNode> left;
  if (!ParsePrefixToken(token.Release(), left.Receive()))
    return false;
  DASSERT(left.ptr());

  while (binding_power < GetBindingPower(look_ahead_token_->type())) {
    if (!GetNextToken(token.Receive()))
      return false;

    memory::scoped_ptr<const ASTNode> node;
    if (!ParseInfixToken(token.Release(), left.Release(), node.Receive()))
      return false;
    DASSERT(node.ptr());

    left.Reset(node.Release());
  }

  *root = left.Release();
  return true;
}

bool Parser::ConsumeToken(int type) {
  memory::scoped_ptr<const Token> token;
  if (!GetNextToken(token.Receive()))
    return false;

  if (!token->IsType(type)) {
    position_ = token->position();
    error_ = string::Format("Unexpected token: %s", token->value().c_str());
    return false;
  }

  return true;
}
