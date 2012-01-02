#ifndef CHAPARRAL_EXECUTER_EXECUTER_H_
#define CHAPARRAL_EXECUTER_EXECUTER_H_

#include "bonavista/util/macros.h"
#include "chaparral/lexer/token.h"

class ASTNode;
class Parser;
class Variant;

class Executer {
 public:
  Executer(Parser* parser);
  virtual ~Executer();

  bool Execute(const Variant** var);

  const Token::Position& position() const;
  const std::string& error() const;

 protected:
  // |node| is owned by the caller.
  virtual bool ExecuteASTNode(const ASTNode* node, const Variant** var) =0;

  Token::Position position_;
  std::string error_;

 private:
  Parser* parser_;

  DISALLOW_COPY_AND_ASSIGN(Executer);
};

#endif
