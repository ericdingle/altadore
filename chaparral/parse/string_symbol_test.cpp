#include "chaparral/parse/string_symbol.h"

#include "bonavista/testing/test_case.h"

class Parser {
};

TEST_CASE(StringSymbolTest) {
};

TEST(StringSymbolTest, Parse) {
  PrefixSymbolRef symbol(new StringSymbol(new Token(Token::STRING, "asdf", 1, 1)));
  Parser parser;
  SymbolRef root;
  EXPECT_TRUE(symbol->Parse(&parser, &root));
  EXPECT_EQ(root.ptr(), symbol.ptr());
}
