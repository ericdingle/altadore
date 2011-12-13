#include "chaparral/parser/ast_node.h"

#include "bonavista/base/types.h"
#include "chaparral/lexer/token.h"

ASTNode::ASTNode(const Token* token) : token_(token) {
}

ASTNode::~ASTNode() {
  for (uint i = 0; i < children_.size(); ++i)
    delete children_[i];
}

const Token* ASTNode::token() const {
  return token_.ptr();
}

void ASTNode::AddChild(const ASTNode* node) {
  children_.push_back(node);
}

const std::vector<const ASTNode*>& ASTNode::children() const {
  return children_;
}
