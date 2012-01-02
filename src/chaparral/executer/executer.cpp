#include "chaparral/executer/executer.h"

#include "bonavista/memory/scoped_ptr.h"
#include "chaparral/executer/variant.h"
#include "chaparral/parser/ast_node.h"
#include "chaparral/parser/parser.h"

Executer::Executer(Parser* parser) : parser_(parser) {
}

Executer::~Executer() {
}

bool Executer::Execute(const Variant** var) {
  ASSERT(var);

  memory::scoped_ptr<const ASTNode> node;
  if (!parser_->Parse(node.Receive())) {
    position_ = parser_->position();
    error_ = parser_->error();
    return false;
  }

  if (!node.ptr()) {
    *var = NULL;
    return true;
  }

  return ExecuteASTNode(node.ptr(), var);
}

const Token::Position& Executer::position() const {
  return position_;
}

const std::string& Executer::error() const {
  return error_;
}
