#include "chaparral/lexer/token.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(TokenTest) {
};

TEST(TokenTest, Constructor) {
  Token::Position position(12, 54);
  Token token(1, "lazy4", position);
  EXPECT_TRUE(token.IsType(1));
  EXPECT_EQ(token.type(), 1);
  EXPECT_EQ(token.value(), "lazy4");
  EXPECT_EQ(token.position().line, 12);
  EXPECT_EQ(token.position().column, 54);
}
