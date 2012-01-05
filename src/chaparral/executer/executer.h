#ifndef CHAPARRAL_EXECUTER_EXECUTER_H_
#define CHAPARRAL_EXECUTER_EXECUTER_H_

#include "bonavista/memory/scoped_ptr.h"
#include "bonavista/util/macros.h"
#include "chaparral/executer/variant.h"
#include "chaparral/lexer/token.h"
#include "chaparral/parser/ast_node.h"

class Parser;

class Executer {
 public:
  Executer(Parser* parser);
  virtual ~Executer();

  bool Execute(const Variant** var);
  template <typename T>
  bool ExecuteT(T* out);

  const Token::Position& position() const;
  const std::string& error() const;

 protected:
  // |node| is owned by the caller.
  virtual bool ExecuteASTNode(const ASTNode* node, const Variant** var) =0;
  // |node| is owned by the caller.
  template <typename T>
  bool ExecuteASTNodeT(const ASTNode* node, T* out);

  Token::Position position_;
  std::string error_;

 private:
  Parser* parser_;

  DISALLOW_COPY_AND_ASSIGN(Executer);
};

template <typename T>
bool Executer::ExecuteT(T* out) {
  memory::scoped_ptr<const Variant> var;
  if (!Execute(var.Receive()))
    return false;

  if (!var->Get(out)) {
    error_ = "Unexpected result type";
    return false;
  }

  return true;
}

template <typename T>
bool Executer::ExecuteASTNodeT(const ASTNode* node, T* out) {
  memory::scoped_ptr<const Variant> var;
  if (!ExecuteASTNode(node, var.Receive()))
    return false;

  if (!var->Get(out)) {
    position_ = node->token()->position();
    error_ = "Unexpected result type";
    return false;
  }

  return true;
}

#endif
