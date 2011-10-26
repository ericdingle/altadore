#include <stdio.h>
#include <string>
#include "bonavista/io/io.h"
#include "chaparral/json/array_operator.h"
#include "chaparral/json/json_parser.h"
#include "chaparral/json/object_operator.h"

void PrintJsonTree(const Symbol* symbol, int level=0);

int main(int argc, char* argv[]) {
  JsonParser parser;

  if (argc != 2) {
    printf("Usage: json <filename>\n");
    return 1;
  }

  std::string buffer;
  if (!io::ReadFile(argv[1], &buffer)) {
    printf("Error: Could not read file\n");
    return 1;
  }

  TokenStream token_stream(buffer.c_str());
  SymbolRef symbol;
  if (!parser.Parse(&token_stream, &symbol)) {
    printf("Parse Error: %s at line %d, column %d\n", parser.error_message().c_str(), parser.error_location().line, parser.error_location().column);
    return 1;
  }

  if (!token_stream.EndOfInput()) {
    printf("Input Error: Unexpected garbage at end of input\n");
    return 1;
  }

  PrintJsonTree(symbol.ptr());
  printf("\n");

  return 0;
}

void PrintJsonTree(const Symbol* symbol, int level) {
  std::string indent(2 * level, ' ');

  const ArrayOperator* arr = dynamic_cast<const ArrayOperator*>(symbol);
  if (arr) {
    printf("[");
    const SymbolRefList& symbols = arr->symbols();
    const char* comma = "";
    for (unsigned int i = 0; i < symbols.size(); i++) {
      printf("%s\n%s  ", comma, indent.c_str());
      comma = ",";
      PrintJsonTree(symbols[i].ptr(), level + 1);
    }
    printf("\n%s]", indent.c_str());
    return;
  }

  const ObjectOperator* obj = dynamic_cast<const ObjectOperator*>(symbol);
  if (obj) {
    printf("{");
    const ObjectOperator::SymbolRefMap& symbol_map = obj->symbol_map();
    const char* comma = "";
    for(ObjectOperator::SymbolRefMap::const_iterator iter = symbol_map.begin(); iter != symbol_map.end(); iter++) {
      printf("%s\n%s  %s: ", comma, indent.c_str(), iter->first.c_str());
      comma = ",";
      PrintJsonTree(iter->second.ptr(), level + 1);
    }
    printf("\n%s}", indent.c_str());
    return;
  }

  printf("%s", symbol->value().c_str());
}
