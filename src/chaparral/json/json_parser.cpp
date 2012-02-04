#include "chaparral/json/json_parser.h"

#include <vector>
#include "bonavista/string/format.h"
#include "chaparral/json/json_lexer.h"
#include "chaparral/parser/ast_node.h"

JsonParser::JsonParser(TokenStream* token_stream) : Parser(token_stream) {
}

JsonParser::~JsonParser() {
}

bool JsonParser::Parse(const ASTNode** root) {
  ASSERT(root);

  memory::scoped_ptr<const ASTNode> node;
  if (!Parser::Parse(node.Receive()))
    return false;
  if (!node.ptr()) {
    error_ = "Input is empty.";
    return false;
  }

  memory::scoped_ptr<const ASTNode> dummy;
  if (!Parser::Parse(dummy.Receive()) || dummy.ptr()) {
    error_ = "Encountered more than one expression.";
    return false;
  }

  *root = node.Release();
  return true;
}

bool JsonParser::ParsePrefixToken(const Token* token, const ASTNode** root) {
  ASSERT(token);
  ASSERT(root);

  memory::scoped_ptr<const Token> token_holder(token);

  if (token->IsType(JsonLexer::TYPE_LEFT_BRACE))
    return ParseObject(token_holder.Release(), root);
  else if (token->IsType(JsonLexer::TYPE_LEFT_BRACKET))
    return ParseArray(token_holder.Release(), root);

  if (token->IsType(JsonLexer::TYPE_FALSE) ||
      token->IsType(JsonLexer::TYPE_NULL) ||
      token->IsType(JsonLexer::TYPE_NUMBER) ||
      token->IsType(JsonLexer::TYPE_STRING) ||
      token->IsType(JsonLexer::TYPE_TRUE)) {
    *root = new ASTNode(token_holder.Release());
    return true;
  }

  position_ = token->position();
  error_ = string::Format("Unexpected token: %s", token->value().c_str());
  return false;
}

bool JsonParser::ParseObject(const Token* token, const ASTNode** root) {
  // Implements:
  //  object -> '{' pairs '}'
  //  pair -> string ':' value
  //  pairs -> pair more_pairs | E
  //  more_pairs -> ',' pair more_pairs | E
  ASSERT(root);

  memory::scoped_ptr<ASTNode> node(new ASTNode(token));

  if (!look_ahead_token_->IsType(JsonLexer::TYPE_RIGHT_BRACE)) {
    while (true) {
      memory::scoped_ptr<const ASTNode> key;
      if (!ParseExpression(0, key.Receive()))
        return false;

      if (!key->token()->IsType(JsonLexer::TYPE_STRING)) {
        position_ = token->position();
        error_ = string::Format("Expecting string but found %s",
                                token->value().c_str());
        return false;
      }

      node->AddChild(key.Release());

      if (!ConsumeToken(JsonLexer::TYPE_COLON))
        return false;

      memory::scoped_ptr<const ASTNode> value;
      if (!ParseExpression(0, value.Receive()))
        return false;

      node->AddChild(value.Release());

      if (!look_ahead_token_->IsType(JsonLexer::TYPE_COMMA))
        break;

      ASSERT(ConsumeToken(JsonLexer::TYPE_COMMA));
    }
  }

  if (!ConsumeToken(JsonLexer::TYPE_RIGHT_BRACE))
    return false;

  *root = node.Release();
  return true;
}

bool JsonParser::ParseArray(const Token* token, const ASTNode** root) {
  // Implements:
  //   array -> '[' values ']'
  //   values -> value more_values | E
  //   more_values -> ',' value more_values | E
  ASSERT(root);

  memory::scoped_ptr<ASTNode> node(new ASTNode(token));

  if (!look_ahead_token_->IsType(JsonLexer::TYPE_RIGHT_BRACKET)) {
    while (true) {
      memory::scoped_ptr<const ASTNode> value;
      if (!ParseExpression(0, value.Receive()))
        return false;

      node->AddChild(value.Release());

      if (!look_ahead_token_->IsType(JsonLexer::TYPE_COMMA))
        break;

      ASSERT(ConsumeToken(JsonLexer::TYPE_COMMA));
    }
  }

  if (!ConsumeToken(JsonLexer::TYPE_RIGHT_BRACKET))
    return false;

  *root = node.Release();
  return true;
}
