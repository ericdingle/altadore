#include "altadore/scene_interp/scene_parser.h"

#include "altadore/scene_interp/scene_lexer.h"
#include "bonavista/base/macros.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(SceneParserTest) {
 protected:
  void Init(const char* input) {
    stream_.Reset(new TokenStream(&lexer_, input));
    parser_.Reset(new SceneParser(stream_.ptr()));
  }

  SceneLexer lexer_;
  scoped_ptr<TokenStream> stream_;
  scoped_ptr<Parser> parser_;
  scoped_ptr<const ASTNode> root_;
};

TEST(SceneParserTest, ParseEmpty) {
  Init("");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
  EXPECT_NULL(root_.ptr());
}

TEST(SceneParserTest, ParseUnknown) {
  Init("*");
  EXPECT_FALSE(parser_->Parse(root_.Receive()));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(SceneParserTest, ParseError) {
  Init("var1");
  EXPECT_FALSE(parser_->Parse(root_.Receive()));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(SceneParserTest, ParseIdentifier) {
  Init("var1;");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
}

TEST(SceneParserTest, ParseNumber) {
  Init("1.1;");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_NUMBER));
}

TEST(SceneParserTest, ParseNewObject) {
  Init("new a();");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_NEW));
  EXPECT_EQ(root_->children().size(), 1);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));
}

TEST(SceneParserTest, ParseNewObjectError) {
  Init("new 1;");
  EXPECT_FALSE(parser_->Parse(root_.Receive()));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(SceneParserTest, ParseDotAccessor) {
  Init("a.b();");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
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
    Init(inputs[i]);
    EXPECT_FALSE(parser_->Parse(root_.Receive()));
    EXPECT_FALSE(parser_->error().empty());
  }
}

TEST(SceneParserTest, ParseAssignment) {
  Init("a = 5;");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
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
    Init("5 = 5;");
    EXPECT_FALSE(parser_->Parse(root_.Receive()));
    EXPECT_FALSE(parser_->error().empty());
  }
}

TEST(SceneParserTest, ParseFunction) {
  Init("a.b();");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));

  EXPECT_EQ(root_->children().size(), 2);
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));

  const std::vector<const ASTNode*>& children1 = root_->children()[1]->children();
  EXPECT_EQ(children1.size(), 1);
  EXPECT_TRUE(children1[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));

  Init("a.b(1);");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_DOT));

  EXPECT_EQ(root_->children().size(), 2);
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));

  const std::vector<const ASTNode*>& children2 = root_->children()[1]->children();
  EXPECT_EQ(children2.size(), 2);
  EXPECT_TRUE(children2[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(children2[1]->token()->IsType(SceneLexer::TYPE_NUMBER));

  Init("a.b(1, 2);");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
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
    Init("a();");
    EXPECT_FALSE(parser_->Parse(root_.Receive()));
    EXPECT_FALSE(parser_->error().empty());
  }
}
