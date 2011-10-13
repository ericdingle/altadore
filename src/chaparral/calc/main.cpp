#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include "chaparral/calc/calc_parser.h"
#include "chaparral/execute/executer.h"
#include "chaparral/token/token_stream.h"

namespace {

const char* INPUT_PREFIX = "calc > ";

}  // namespace

int main() {
  CalcParser parser;

  while (true) {
    printf("%s", INPUT_PREFIX);
    std::string input;
    getline(std::cin, input);
    if (input.empty())
      break;

    TokenStream token_stream(input.c_str());
    SymbolRef symbol;
    if (!parser.Parse(&token_stream, &symbol)) {
      int offset = static_cast<int>(strlen(INPUT_PREFIX) + parser.error_location().column);
      printf("%*s\n", offset, "^");
      printf("Parse error: %s.\n", parser.error_message().c_str());
      continue;
    }

    if (!token_stream.EndOfInput()) {
      printf("Input error: garbage at end of input.\n");
      continue;
    }

    Executer executer;
    memory::scoped_ref<Value> result;
    if (!executer.Execute(symbol.ptr(), &result)) {
      printf("Execute error: %s\n", executer.error_message().c_str());
      continue;
    }

    printf("Result: %s\n", result->ToString().c_str());
  }

  return 0;
}
