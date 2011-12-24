#include "chaparral/calc/calc_lexer.h"

#include "bonavista/util/macros.h"
#include "bonavista/testing/test_case.h"


TEST_CASE(CalcLexerTest) {
 protected:
  CalcLexer lexer_;

  std::string input_;
  int type_;
  std::string value_;
  uint count_;
  std::string error_;
};

TEST(CalcLexerTest, Operators) {
  const char* inputs[] = { "*", "(", "-", "+", ")", "/" };
  const int types[] = { CalcLexer::TYPE_ASTERISK,
                        CalcLexer::TYPE_LEFT_PARENTHESIS,
                        CalcLexer::TYPE_MINUS,
                        CalcLexer::TYPE_PLUS,
                        CalcLexer::TYPE_RIGHT_PARENTHESIS,
                        CalcLexer::TYPE_SLASH };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(type_, types[i]);
    EXPECT_EQ(value_, input_);
    EXPECT_EQ(count_, 1);
  }

  input_ = ".";
  EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_FALSE(error_.empty());
}

TEST(CalcLexerTest, Numbers) {
  const char* inputs[] = {
    "0", "1", "12", "123",
    "0.1", "12.3", "12.34",
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(type_, CalcLexer::TYPE_NUMBER);
    EXPECT_EQ(value_, input_);
    EXPECT_EQ(count_, input_.length());
  }

  const char* bad_inputs[] = { "01", "1." };

  for (uint i = 0; i < ARRAY_SIZE(bad_inputs); ++i) {
    input_ = bad_inputs[i];
    error_.clear();
    EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_FALSE(error_.empty());
  }
}
