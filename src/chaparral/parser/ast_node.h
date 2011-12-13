#ifndef CHAPARRAL_PARSER_AST_NODE_H_
#define CHAPARRAL_PARSER_AST_NODE_H_

#include <vector>
#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"
#include "bonavista/memory/scoped_ptr.h"

class Token;

class ASTNode {
 public:
  ASTNode(const Token* token);
  ~ASTNode();

  const Token* token() const;

  void AddChild(const ASTNode* node);
  const std::vector<const ASTNode*>& children() const;

 private:
  const memory::scoped_ptr<const Token> token_;
  std::vector<const ASTNode*> children_;

  DISALLOW_COPY_AND_ASSIGN(ASTNode);
};

#endif
