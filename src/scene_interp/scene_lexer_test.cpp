#include "scene_interp/scene_lexer.h"

#include <tuple>
#include <vector>
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class SceneLexerTest : public testing::Test {
 protected:
  SceneLexer lexer_;

  std::string input_;
  int type_;
  std::string value_;
  int count_;
  std::string error_;
};

TEST_F(SceneLexerTest, TokenizeUnknown) {
  input_ = "*";
  EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_FALSE(error_.empty());
}

TEST_F(SceneLexerTest, TokenizeOperator) {
  std::vector<std::tuple<const char*, SceneLexer::Type>> test_cases = {
      std::make_tuple(",", SceneLexer::TYPE_COMMA),
      std::make_tuple(".", SceneLexer::TYPE_DOT),
      std::make_tuple("=", SceneLexer::TYPE_EQUAL),
      std::make_tuple("(", SceneLexer::TYPE_LEFT_PARENTHESIS),
      std::make_tuple(")", SceneLexer::TYPE_RIGHT_PARENTHESIS),
      std::make_tuple(";", SceneLexer::TYPE_SEMI_COLON),
      };

  for (const auto& test_case : test_cases) {
    input_ = std::get<0>(test_case);
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(std::get<1>(test_case), type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(1, count_);
  }
}

TEST_F(SceneLexerTest, TokenizeNew) {
  input_ = "new";
  EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_EQ(SceneLexer::TYPE_NEW, type_);
  EXPECT_EQ(input_, value_);
  EXPECT_EQ(input_.length(), count_);
}

TEST_F(SceneLexerTest, TokenizeIdentifier) {
  input_ = "adf9f3q_ue0";
  EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_EQ(SceneLexer::TYPE_IDENTIFIER, type_);
  EXPECT_EQ(input_, value_);
  EXPECT_EQ(input_.length(), count_);
}

TEST_F(SceneLexerTest, TokenizeNumber) {
  std::vector<const char*> test_cases = {
    "0", "1", "12", "123",
    "0.1", "12.3", "12.34",
    "-1", "-12.3"
  };

  for (const char* test_case : test_cases) {
    input_ = test_case;
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(SceneLexer::TYPE_NUMBER, type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST_F(SceneLexerTest, TokenizeNumberError) {
  std::vector<const char*> test_cases = { "01", "1.", "-", "-a" };

  for (const char* test_case : test_cases) {
    input_ = test_case;
    error_.clear();
    EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_FALSE(error_.empty());
  }
}
