#include "chaparral/parse/number_symbol.h"

#include "bonavista/testing/test_case.h"

class Parser {
};

TEST_CASE(NumberSymbolTest) {
};

TEST(NumberSymbolTest, Parse) {
  PrefixSymbolRef symbol(new NumberSymbol(new Token(Token::NUMBER, "5.5", 1, 1)));
  Parser parser;
  SymbolRef root;
  EXPECT_TRUE(symbol->Parse(&parser, &root));
  EXPECT_EQ(root.ptr(), symbol.ptr());
}
