#include "altadore/scene_interp/scene_lexer.h"

#include <utility>

#include "bonavista/lexer/lexer_test_fixture.h"
#include "bonavista/lexer/token_test_macros.h"

class SceneLexerTest : public LexerTestFixture<SceneLexer> {
};

TEST_F(SceneLexerTest, GetTokenUnexpected) {
  EXPECT_EQ(GetToken("*").status().message(), "Unexpected character '*' at 1:2.");
}

TEST_F(SceneLexerTest, GetTokenOperator) {
  std::pair<const char*, SceneLexer::Type> test_cases[] = {
      {",", SceneLexer::TYPE_COMMA},
      {".", SceneLexer::TYPE_DOT},
      {"=", SceneLexer::TYPE_EQUAL},
      {"(", SceneLexer::TYPE_LEFT_PARENTHESIS},
      {")", SceneLexer::TYPE_RIGHT_PARENTHESIS},
      {";", SceneLexer::TYPE_SEMI_COLON},
      };

  for (const auto& test_case : test_cases) {
    EXPECT_TOKEN(*GetToken(test_case.first).value(), test_case.second,
                 test_case.first, 1, 2);
  }
}

TEST_F(SceneLexerTest, GetTokenIdentifier) {
  EXPECT_TOKEN(*GetToken("adf9f3q_ue0$").value(), SceneLexer::TYPE_IDENTIFIER,
               "adf9f3q_ue0", 1, 2);
}

TEST_F(SceneLexerTest, GetTokenNumber) {
  const char* test_cases[] = {
    "0", "1", "12", "123",
    "0.1", "12.3", "12.34",
    "-1", "-12.3"
  };

  for (const char* test_case : test_cases) {
    EXPECT_TOKEN(*GetToken(test_case).value(), SceneLexer::TYPE_NUMBER,
                 test_case, 1, 2);
  }
}

TEST_F(SceneLexerTest, GetTokenNumberError) {
  std::pair<const char*, const char*> test_cases[] = {
      {"1.", "Unexpected character '(end of input)' at 1:2."},
      {"-", "Unexpected character '(end of input)' at 1:2."},
      {"-a", "Unexpected character 'a' at 1:2."},
      };

  for (const auto& test_case : test_cases) {
    EXPECT_EQ(GetToken(test_case.first).status().message(), test_case.second);
  }
}
