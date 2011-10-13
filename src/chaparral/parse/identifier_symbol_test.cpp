#include "chaparral/parse/identifier_symbol.h"

#include "bonavista/testing/test_case.h"

class Parser {
};

TEST_CASE(IdentifierSymbolTest) {
};

TEST(IdentifierSymbolTest, Parse) {
  PrefixSymbolRef symbol(new IdentifierSymbol(new Token(Token::IDENTIFIER, "test", 1, 1)));
  Parser parser;
  SymbolRef root;
  EXPECT_TRUE(symbol->Parse(&parser, &root));
  EXPECT_EQ(root.ptr(), symbol.ptr());
}
