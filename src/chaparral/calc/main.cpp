#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include "chaparral/calc/calc_lexer.h"
#include "chaparral/calc/calc_parser.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

namespace {

const char* kInputPrefix = "calc > ";

}  // namespace

int main() {
  CalcLexer lexer;
  CalcParser parser;

  while (true) {
    printf("%s", kInputPrefix);
    std::string input;
    getline(std::cin, input);
    if (input.empty())
      break;

    TokenStream token_stream(&lexer, input);
    memory::scoped_ptr<const ASTNode> root;
    if (!parser.Parse(&token_stream, root.Receive())) {
      int offset = static_cast<int>(strlen(kInputPrefix) + parser.position().column);
      printf("%*s\n", offset, "^");
      printf("Parse error: %s.\n", parser.error().c_str());
      continue;
    }
  }

  return 0;
}
