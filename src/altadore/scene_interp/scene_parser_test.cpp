#include "altadore/scene_interp/scene_parser.h"

#include "altadore/scene_interp/scene_lexer.h"
#include "bonavista/testing/test_case.h"
#include "bonavista/util/macros.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(SceneParserTest) {
 protected:
  SceneLexer lexer_;
  memory::scoped_ptr<const ASTNode> root_;
};

TEST(SceneParserTest, ParseEmpty) {
  TokenStream stream(&lexer_, "");
  SceneParser parser(&stream);
  EXPECT_TRUE(parser.Parse(root_.Receive()));
  EXPECT_NULL(root_.ptr());
}

TEST(SceneParserTest, ParseUnknown) {
  TokenStream stream(&lexer_, "*");
  SceneParser parser(&stream);
  EXPECT_FALSE(parser.Parse(root_.Receive()));
  EXPECT_FALSE(parser.error().empty());
}

TEST(SceneParserTest, ParseError) {
  TokenStream stream(&lexer_, "var1");
  SceneParser parser(&stream);
  EXPECT_FALSE(parser.Parse(root_.Receive()));
  EXPECT_FALSE(parser.error().empty());
}

TEST(SceneParserTest, ParseIdentifier) {
  TokenStream stream(&lexer_, "var1;");
  SceneParser parser(&stream);
  EXPECT_TRUE(parser.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
}

TEST(SceneParserTest, ParseNumber) {
  TokenStream stream(&lexer_, "1.1;");
  SceneParser parser(&stream);
  EXPECT_TRUE(parser.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_NUMBER));
}

TEST(SceneParserTest, ParseDotAccessor) {
  TokenStream stream(&lexer_, "a.b();");
  SceneParser parser(&stream);
  EXPECT_TRUE(parser.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));
  EXPECT_EQ(root_->children().size(), 2);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));
}

TEST(SceneParserTest, ParseDotAccessorError) {
  const char* inputs[] = {
    "1.b();",
    "a.*;",
    "a.5;"
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    TokenStream stream(&lexer_, inputs[i]);
    SceneParser parser(&stream);
    EXPECT_FALSE(parser.Parse(root_.Receive()));
    EXPECT_FALSE(parser.error().empty());
  }
}

TEST(SceneParserTest, ParseAssignment) {
  TokenStream stream(&lexer_, "a = 5;");
  SceneParser parser(&stream);
  EXPECT_TRUE(parser.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_EQUAL));
  EXPECT_EQ(root_->children().size(), 2);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_NUMBER));
}

TEST(SceneParserTest, ParseAssignmentError) {
  const char* inputs[] = {
    "5 = 5;",
    "a = *;"
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    TokenStream stream(&lexer_, "5 = 5;");
    SceneParser parser(&stream);
    EXPECT_FALSE(parser.Parse(root_.Receive()));
    EXPECT_FALSE(parser.error().empty());
  }
}

TEST(SceneParserTest, ParseFunction) {
  TokenStream stream1(&lexer_, "a.b();");
  SceneParser parser1(&stream1);
  EXPECT_TRUE(parser1.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));

  EXPECT_EQ(root_->children().size(), 2);
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));

  const std::vector<const ASTNode*>& children1 = root_->children()[1]->children();
  EXPECT_EQ(children1.size(), 1);
  EXPECT_TRUE(children1[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));

  TokenStream stream2(&lexer_, "a.b(1);");
  SceneParser parser2(&stream2);
  EXPECT_TRUE(parser2.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));

  EXPECT_EQ(root_->children().size(), 2);
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));

  const std::vector<const ASTNode*>& children2 = root_->children()[1]->children();
  EXPECT_EQ(children2.size(), 2);
  EXPECT_TRUE(children2[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(children2[1]->token()->IsType(SceneLexer::TYPE_NUMBER));

  TokenStream stream3(&lexer_, "a.b(1, 2);");
  SceneParser parser3(&stream3);
  EXPECT_TRUE(parser3.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));

  EXPECT_EQ(root_->children().size(), 2);
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));

  const std::vector<const ASTNode*>& children3 = root_->children()[1]->children();
  EXPECT_EQ(children3.size(), 3);
  EXPECT_TRUE(children3[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(children3[1]->token()->IsType(SceneLexer::TYPE_NUMBER));
  EXPECT_TRUE(children3[2]->token()->IsType(SceneLexer::TYPE_NUMBER));
}

TEST(SceneParserTest, ParseFunctionError) {
  const char* inputs[] = {
    "a();",
    "a.5();",
    "a.b(*);",
    "a.b(5;",
    "a.b(5,);"
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    TokenStream stream(&lexer_, "a();");
    SceneParser parser(&stream);
    EXPECT_FALSE(parser.Parse(root_.Receive()));
    EXPECT_FALSE(parser.error().empty());
  }
}
