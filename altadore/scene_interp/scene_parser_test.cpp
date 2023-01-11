#include "altadore/scene_interp/scene_parser.h"

#include "altadore/scene_interp/scene_lexer.h"
#include "bonavista/lexer/token_test_macros.h"
#include "bonavista/parser/parser_test_fixture.h"

class SceneParserTest : public ParserTestFixture<SceneLexer, SceneParser> {
};

TEST_F(SceneParserTest, ParseEmpty) {
  EXPECT_EQ(Parse("").status().message(),
            "Unexpected token '(end of input)' at 1:1.");
}

TEST_F(SceneParserTest, ParseUnknown) {
  EXPECT_EQ(Parse("*").status().message(), "Unexpected character '*' at 1:1.");
}

TEST_F(SceneParserTest, ParseError) {
  EXPECT_EQ(Parse("var1").status().message(),
            "Unexpected token '(end of input)' at 1:5.");
}

TEST_F(SceneParserTest, ParseIdentifier) {
  EXPECT_TOKEN(Parse("var1;").value()->token(), SceneLexer::TYPE_IDENTIFIER,
               "var1", 1, 1);
}

TEST_F(SceneParserTest, ParseNumber) {
  EXPECT_TOKEN(Parse("1.1;").value()->token(), SceneLexer::TYPE_NUMBER, "1.1", 1, 1);
}

TEST_F(SceneParserTest, ParseDotAccessor) {
  auto node = Parse("a.b;").value();
  EXPECT_TOKEN(node->token(), SceneLexer::TYPE_DOT, ".", 1, 2);

  const auto& children = node->children();
  EXPECT_EQ(2U, node->children().size());
  EXPECT_TOKEN(children[0]->token(), SceneLexer::TYPE_IDENTIFIER, "a", 1, 1);
  EXPECT_TOKEN(children[1]->token(), SceneLexer::TYPE_IDENTIFIER, "b", 1, 3);
}

TEST_F(SceneParserTest, ParseDotAccessorError) {
  std::pair<const char*, const char*> test_cases[] = {
      {"a.*;", "Unexpected character '*' at 1:3."},
      {"a.5;", "Unexpected token '5' at 1:3."}
      };

  for (const auto& test_case : test_cases) {
    EXPECT_EQ(Parse(test_case.first).status().message(), test_case.second);
  }
}

TEST_F(SceneParserTest, ParseAssignment) {
  auto node = Parse("a = 5;").value();
  EXPECT_TOKEN(node->token(), SceneLexer::TYPE_EQUAL, "=", 1, 3);

  const auto& children = node->children();
  EXPECT_EQ(2U, node->children().size());
  EXPECT_TOKEN(children[0]->token(), SceneLexer::TYPE_IDENTIFIER, "a", 1, 1);
  EXPECT_TOKEN(children[1]->token(), SceneLexer::TYPE_NUMBER, "5", 1, 5);
}

TEST_F(SceneParserTest, ParseAssignmentError) {
  std::pair<const char*, const char*> test_cases[] = {
      {"    5 = 4;", "Unexpected token '5' at 1:5."},
      {"a = );", "Unexpected token ')' at 1:5."},
      };

  for (const auto& test_case : test_cases) {
    EXPECT_EQ(Parse(test_case.first).status().message(), test_case.second);
  }
}

TEST_F(SceneParserTest, ParseFunction) {
  auto node = Parse("a();").value();
  EXPECT_TOKEN(node->token(), SceneLexer::TYPE_LEFT_PARENTHESIS, "(", 1, 2);
  const auto& children = node->children();
  EXPECT_EQ(1U, node->children().size());
  EXPECT_TOKEN(children[0]->token(), SceneLexer::TYPE_IDENTIFIER, "a", 1, 1);

  node = Parse("a(1);").value();
  EXPECT_TOKEN(node->token(), SceneLexer::TYPE_LEFT_PARENTHESIS, "(", 1, 2);
  const auto& children2 = node->children();
  EXPECT_EQ(2U, node->children().size());
  EXPECT_TOKEN(children2[0]->token(), SceneLexer::TYPE_IDENTIFIER, "a", 1, 1);
  EXPECT_TOKEN(children2[1]->token(), SceneLexer::TYPE_NUMBER, "1", 1, 3);

  node = Parse("a(1, b);").value();
  EXPECT_TOKEN(node->token(), SceneLexer::TYPE_LEFT_PARENTHESIS, "(", 1, 2);
  const auto& children3 = node->children();
  EXPECT_EQ(3U, node->children().size());
  EXPECT_TOKEN(children3[0]->token(), SceneLexer::TYPE_IDENTIFIER, "a", 1, 1);
  EXPECT_TOKEN(children3[1]->token(), SceneLexer::TYPE_NUMBER, "1", 1, 3);
  EXPECT_TOKEN(children3[2]->token(), SceneLexer::TYPE_IDENTIFIER, "b", 1, 6);
}

TEST_F(SceneParserTest, ParseFunctionError) {
  std::pair<const char*, const char*> test_cases[] = {
      {"a(  =)", "Unexpected token '=' at 1:5."},
      {"a(4 5)", "Unexpected token '5' at 1:5."},
      {"a(4 ", "Unexpected token '(end of input)' at 1:5."},
      };

  for (const auto& test_case : test_cases) {
    EXPECT_EQ(Parse(test_case.first).status().message(), test_case.second);
  }
}

TEST_F(SceneParserTest, ParseBindingBower) {
  auto node = Parse("a = b.c();").value();
  EXPECT_TOKEN(node->token(), SceneLexer::TYPE_EQUAL, "=", 1, 3);
  EXPECT_EQ(2U, node->children().size());
  EXPECT_TOKEN(node->children()[0]->token(), SceneLexer::TYPE_IDENTIFIER, "a", 1, 1);

  const Node* node1 = node->children()[1].get();
  EXPECT_TOKEN(node1->token(), SceneLexer::TYPE_LEFT_PARENTHESIS, "(", 1, 8);
  EXPECT_EQ(1U, node1->children().size());

  node1 = node1->children()[0].get();
  EXPECT_TOKEN(node1->token(), SceneLexer::TYPE_DOT, ".", 1, 6);
  EXPECT_EQ(2U, node1->children().size());
  EXPECT_TOKEN(node1->children()[0]->token(), SceneLexer::TYPE_IDENTIFIER, "b", 1, 5);
  EXPECT_TOKEN(node1->children()[1]->token(), SceneLexer::TYPE_IDENTIFIER, "c", 1, 7);
}
