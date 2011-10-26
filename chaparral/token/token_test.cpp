#include "chaparral/token/token.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(TokenTest) {
};

TEST(TokenTest, Constructor) {
  Token token(Token::IDENTIFIER, "lazy4", 12, 54);
  EXPECT_TRUE(token.IsType(Token::IDENTIFIER));
  EXPECT_EQ(token.type(), Token::IDENTIFIER);
  EXPECT_EQ(token.value(), "lazy4");
  EXPECT_EQ(token.location().line, 12);
  EXPECT_EQ(token.location().column, 54);
}
