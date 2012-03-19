#include "chaparral/executer/executer.h"

#include "chaparral/parser/parser.h"

Executer::Executer(Parser* parser) : parser_(parser) {
}

Executer::~Executer() {
}

bool Executer::Execute(const Variant** var) {
  DCHECK(var);

  scoped_ptr<const ASTNode> node;
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

bool Executer::ExecuteAll() {
  scoped_refptr<const Variant> var;

  do {
    if (!Execute(var.Receive()))
      return false;
  } while (var.ptr() != NULL);

  return true;
}

const Token::Position& Executer::position() const {
  return position_;
}

const std::string& Executer::error() const {
  return error_;
}
