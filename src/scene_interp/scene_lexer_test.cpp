#include "scene_interp/scene_lexer.h"

#include <utility>
#include "third_party/bonavista/src/lexer/lexer_test_fixture.h"
#include "third_party/bonavista/src/lexer/token_test_macros.h"
#include "third_party/bonavista/src/util/status_test_macros.h"

class SceneLexerTest : public LexerTestFixture<SceneLexer> {
};

TEST_F(SceneLexerTest, GetTokenUnexpected) {
  EXPECT_STATUS(GetToken("*").status(), "Unexpected character: *", 1, 2);
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
      {"1.", "Unexpected end of input"},
      {"-", "Unexpected end of input"},
      {"-a", "Unexpected character: a"},
      };

  for (const auto& test_case : test_cases) {
    EXPECT_STATUS(GetToken(test_case.first).status(), test_case.second, 1, 2);
  }
}
