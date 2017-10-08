#include "scene_interp/scene_parser.h"

#include <tuple>
#include <vector>
#include "scene_interp/scene_lexer.h"
#include "third_party/bonavista/src/lexer/token_stream.h"
#include "third_party/bonavista/src/parser/ast_node.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class SceneParserTest : public testing::Test {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new SceneParser(stream_.get()));
  }

  SceneLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<const ASTNode> root_;
};

TEST_F(SceneParserTest, ParseEmpty) {
  Init("");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_EQ(nullptr, root_.get());
}

TEST_F(SceneParserTest, ParseUnknown) {
  Init("*");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(SceneParserTest, ParseError) {
  Init("var1");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(SceneParserTest, ParseIdentifier) {
  Init("var1;");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
}

TEST_F(SceneParserTest, ParseNumber) {
  Init("1.1;");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_NUMBER));
}

TEST_F(SceneParserTest, ParseNewObject) {
  Init("new a();");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_NEW));
  EXPECT_EQ(1, root_->children().size());

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));
}

TEST_F(SceneParserTest, ParseNewObjectError) {
  Init("new 1;");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(SceneParserTest, ParseDotAccessor) {
  Init("a.b();");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));
  EXPECT_EQ(2, root_->children().size());

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));
}

TEST_F(SceneParserTest, ParseDotAccessorError) {
  std::vector<const char*> test_cases = { "1.b();", "a.*;", "a.5;" };

  for (const char* test_case : test_cases) {
    Init(test_case);
    EXPECT_FALSE(parser_->Parse(&root_));
    EXPECT_FALSE(parser_->error().empty());
  }
}

TEST_F(SceneParserTest, ParseAssignment) {
  Init("a = 5;");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_EQUAL));
  EXPECT_EQ(2, root_->children().size());

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_NUMBER));
}

TEST_F(SceneParserTest, ParseAssignmentError) {
  std::vector<const char*> test_cases = { "5 = 5;", "a = *;" };

  for (const char* test_case : test_cases) {
    Init(test_case);
    EXPECT_FALSE(parser_->Parse(&root_));
    EXPECT_FALSE(parser_->error().empty());
  }
}

TEST_F(SceneParserTest, ParseFunction) {
  Init("a.b();");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));

  EXPECT_EQ(2, root_->children().size());
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));

  const std::vector<std::unique_ptr<const ASTNode>>& children1 =
      root_->children()[1]->children();
  EXPECT_EQ(1, children1.size());
  EXPECT_TRUE(children1[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));

  Init("a.b(1);");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));

  EXPECT_EQ(2, root_->children().size());
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));

  const std::vector<std::unique_ptr<const ASTNode>>& children2 =
      root_->children()[1]->children();
  EXPECT_EQ(2, children2.size());
  EXPECT_TRUE(children2[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(children2[1]->token()->IsType(SceneLexer::TYPE_NUMBER));

  Init("a.b(1, 2);");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));

  EXPECT_EQ(2, root_->children().size());
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));

  const std::vector<std::unique_ptr<const ASTNode>>& children3 =
      root_->children()[1]->children();
  EXPECT_EQ(3, children3.size());
  EXPECT_TRUE(children3[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(children3[1]->token()->IsType(SceneLexer::TYPE_NUMBER));
  EXPECT_TRUE(children3[2]->token()->IsType(SceneLexer::TYPE_NUMBER));
}

TEST_F(SceneParserTest, ParseFunctionError) {
  std::vector<const char*> test_cases = {
    "a();",
    "a.5();",
    "a.b(*);",
    "a.b(5;",
    "a.b(5,);",
    "a.b(5,*);"
  };

  for (const char* test_case : test_cases) {
    Init(test_case);
    EXPECT_FALSE(parser_->Parse(&root_));
    EXPECT_FALSE(parser_->error().empty());
  }
}
