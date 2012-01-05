#include "chaparral/calc/calc_executer.h"

#include "bonavista/string/format.h"
#include "chaparral/calc/calc_lexer.h"
#include "chaparral/parser/ast_node.h"

CalcExecuter::CalcExecuter(Parser* parser) : Executer(parser) {
}

CalcExecuter::~CalcExecuter() {
}

bool CalcExecuter::ExecuteASTNode(const ASTNode* node, const Variant** var) {
  ASSERT(node);
  ASSERT(var);

  if (node->token()->IsType(CalcLexer::TYPE_ASTERISK) ||
      node->token()->IsType(CalcLexer::TYPE_MINUS) ||
      node->token()->IsType(CalcLexer::TYPE_PLUS) ||
      node->token()->IsType(CalcLexer::TYPE_SLASH)) {
    double left = 0;
    if (!ExecuteASTNodeT(node->children()[0], &left))
      return false;

    double right = 0;
    if (!ExecuteASTNodeT(node->children()[1], &right))
      return false;

    double result = 0;
    if (node->token()->IsType(CalcLexer::TYPE_ASTERISK))
      result = left * right;
    else if (node->token()->IsType(CalcLexer::TYPE_MINUS))
      result = left - right;
    else if (node->token()->IsType(CalcLexer::TYPE_PLUS))
      result = left + right;
    else
      result = left / right;

    *var = new Variant(result);
    return true;
  }

  if (node->token()->IsType(CalcLexer::TYPE_NUMBER)) {
    double value = atof(node->token()->value().c_str());
    *var = new Variant(value);
    return true;
  }

  position_ = node->token()->position();
  error_ = string::Format("Unexpected token: %s",
                          node->token()->value().c_str());
  return false;
}
