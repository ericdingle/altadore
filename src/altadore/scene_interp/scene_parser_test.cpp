#include "altadore/scene_interp/scene_parser.h"

#include "altadore/scene_interp/scene_lexer.h"
#include "bonavista/testing/test_case.h"
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
  TokenStream stream1(&lexer_, "1.b();");
  SceneParser parser1(&stream1);
  EXPECT_FALSE(parser1.Parse(root_.Receive()));
  EXPECT_FALSE(parser1.error().empty());

  TokenStream stream2(&lexer_, "a.*");
  SceneParser parser2(&stream2);
  EXPECT_FALSE(parser2.Parse(root_.Receive()));
  EXPECT_FALSE(parser2.error().empty());

  TokenStream stream3(&lexer_, "a.5");
  SceneParser parser3(&stream3);
  EXPECT_FALSE(parser3.Parse(root_.Receive()));
  EXPECT_FALSE(parser3.error().empty());
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
  TokenStream stream1(&lexer_, "5 = 5;");
  SceneParser parser1(&stream1);
  EXPECT_FALSE(parser1.Parse(root_.Receive()));
  EXPECT_FALSE(parser1.error().empty());

  TokenStream stream2(&lexer_, "a = *;");
  SceneParser parser2(&stream2);
  EXPECT_FALSE(parser2.Parse(root_.Receive()));
  EXPECT_FALSE(parser2.error().empty());
}

TEST(SceneParserTest, ParseFunction) {
  TokenStream stream1(&lexer_, "a();");
  SceneParser parser1(&stream1);
  EXPECT_TRUE(parser1.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));
  EXPECT_EQ(root_->children().size(), 1);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));

  TokenStream stream2(&lexer_, "a(1);");
  SceneParser parser2(&stream2);
  EXPECT_TRUE(parser2.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));
  EXPECT_EQ(root_->children().size(), 2);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_NUMBER));

  TokenStream stream3(&lexer_, "a(1, 2);");
  SceneParser parser3(&stream3);
  EXPECT_TRUE(parser3.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(SceneLexer::TYPE_LEFT_PARENTHESIS));
  EXPECT_EQ(root_->children().size(), 3);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(SceneLexer::TYPE_IDENTIFIER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(SceneLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(SceneLexer::TYPE_NUMBER));
}

TEST(SceneParserTest, ParseFunctionError) {
  TokenStream stream1(&lexer_, "5();");
  SceneParser parser1(&stream1);
  EXPECT_FALSE(parser1.Parse(root_.Receive()));
  EXPECT_FALSE(parser1.error().empty());

  TokenStream stream2(&lexer_, "a(*);");
  SceneParser parser2(&stream2);
  EXPECT_FALSE(parser2.Parse(root_.Receive()));
  EXPECT_FALSE(parser2.error().empty());

  TokenStream stream3(&lexer_, "a(5;");
  SceneParser parser3(&stream3);
  EXPECT_FALSE(parser3.Parse(root_.Receive()));
  EXPECT_FALSE(parser3.error().empty());

  TokenStream stream4(&lexer_, "a(5,);");
  SceneParser parser4(&stream4);
  EXPECT_FALSE(parser4.Parse(root_.Receive()));
  EXPECT_FALSE(parser4.error().empty());
}
