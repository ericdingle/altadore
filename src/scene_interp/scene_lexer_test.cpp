#include "scene_interp/scene_lexer.h"


TEST_CASE(SceneLexerTest) {
 protected:
  SceneLexer lexer_;

  std::string input_;
  int type_;
  std::string value_;
  uint count_;
  std::string error_;
};

TEST(SceneLexerTest, TokenizeUnknown) {
  input_ = "*";
  EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_FALSE(error_.empty());
}

TEST(SceneLexerTest, TokenizeOperator) {
  const char* inputs[] = { ",", ".", "=", "(", ")", ";" };
  const int types[] = { SceneLexer::TYPE_COMMA,
                        SceneLexer::TYPE_DOT,
                        SceneLexer::TYPE_EQUAL,
                        SceneLexer::TYPE_LEFT_PARENTHESIS,
                        SceneLexer::TYPE_RIGHT_PARENTHESIS,
                        SceneLexer::TYPE_SEMI_COLON };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(types[i], type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(1u, count_);
  }
}

TEST(SceneLexerTest, TokenizeNew) {
  input_ = "new";
  EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_EQ(SceneLexer::TYPE_NEW, type_);
  EXPECT_EQ(input_, value_);
  EXPECT_EQ(input_.length(), count_);
}

TEST(SceneLexerTest, TokenizeIdentifier) {
  input_ = "adf9f3q_ue0";
  EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_EQ(SceneLexer::TYPE_IDENTIFIER, type_);
  EXPECT_EQ(input_, value_);
  EXPECT_EQ(input_.length(), count_);
}

TEST(SceneLexerTest, TokenizeNumber) {
  const char* inputs[] = {
    "0", "1", "12", "123",
    "0.1", "12.3", "12.34",
    "-1", "-12.3"
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(SceneLexer::TYPE_NUMBER, type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST(SceneLexerTest, TokenizeNumberError) {
  const char* inputs[] = { "01", "1.", "-", "-a" };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input_ = inputs[i];
    error_.clear();
    EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_FALSE(error_.empty());
  }
}
