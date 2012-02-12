#include "chaparral/calc/calc_executer.h"

#include "bonavista/logging/assert.h"
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
    ASSERT(ExecuteASTNodeT(node->children()[0], &left));

    double right = 0;
    ASSERT(ExecuteASTNodeT(node->children()[1], &right));

    double result = 0;
    if (node->token()->IsType(CalcLexer::TYPE_ASTERISK))
      result = left * right;
    else if (node->token()->IsType(CalcLexer::TYPE_MINUS))
      result = left - right;
    else if (node->token()->IsType(CalcLexer::TYPE_PLUS))
      result = left + right;
    else
      result = left / right;

    memory::scoped_refptr<const Variant> var_ref(new Variant(result));
    *var = var_ref.Release();
    return true;
  }

  if (node->token()->IsType(CalcLexer::TYPE_NUMBER)) {
    double value = atof(node->token()->value().c_str());
    memory::scoped_refptr<const Variant> var_ref(new Variant(value));
    *var = var_ref.Release();
    return true;
  }

  // This should never happen.
  ASSERT(false);
}
