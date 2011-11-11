#include "chaparral/parse/symbol.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(SymbolTest) {
};

TEST(SymbolTest, Constructor) {
  Symbol symbol(new Token(Token::IDENTIFIER, "lazy4", 12, 54));

  EXPECT_TRUE(symbol.IsType(Token::IDENTIFIER));
  EXPECT_EQ(symbol.type(), Token::IDENTIFIER);
  EXPECT_EQ(symbol.value(), "lazy4");
  EXPECT_EQ(symbol.location().line, 12);
  EXPECT_EQ(symbol.location().column, 54);
}
