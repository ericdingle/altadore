#include "chaparral/json/json_parser.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/json/json_lexer.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(JsonParserTest) {
 protected:
  JsonLexer lexer_;
  JsonParser parser_;

  memory::scoped_ptr<const ASTNode> root_;
};

TEST(JsonParserTest, ParseEmpty) {
  TokenStream stream(&lexer_, "");
  EXPECT_FALSE(parser_.Parse(&stream, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}

TEST(JsonParserTest, ParseUnknown) {
  TokenStream stream(&lexer_, "blah");
  EXPECT_FALSE(parser_.Parse(&stream, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}

TEST(JsonParserTest, ParsePrimitive) {
  TokenStream stream1(&lexer_, "false");
  EXPECT_TRUE(parser_.Parse(&stream1, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_FALSE));

  TokenStream stream2(&lexer_, "null");
  EXPECT_TRUE(parser_.Parse(&stream2, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_NULL));

  TokenStream stream3(&lexer_, "1");
  EXPECT_TRUE(parser_.Parse(&stream3, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_NUMBER));

  TokenStream stream4(&lexer_, "\"test\"");
  EXPECT_TRUE(parser_.Parse(&stream4, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_STRING));

  TokenStream stream5(&lexer_, "true");
  EXPECT_TRUE(parser_.Parse(&stream5, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_TRUE));
}

TEST(JsonParserTest, ParseObject) {
  TokenStream stream1(&lexer_, "{}");
  EXPECT_TRUE(parser_.Parse(&stream1, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(root_->children().size(), 0);

  TokenStream stream2(&lexer_, "{\"a\": 1, \"b\": false}");
  EXPECT_TRUE(parser_.Parse(&stream2, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(root_->children().size(), 4);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root_->children()[3]->token()->IsType(JsonLexer::TYPE_FALSE));
}

TEST(JsonParserTest, ParseObjectError) {
  TokenStream bad_stream1(&lexer_, "{, \"a\": 1}");
  EXPECT_FALSE(parser_.Parse(&bad_stream1, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream2(&lexer_, "{1: false}");
  EXPECT_FALSE(parser_.Parse(&bad_stream2, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream3(&lexer_, "{\"a\", false}");
  EXPECT_FALSE(parser_.Parse(&bad_stream3, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream4(&lexer_, "{\"a\": }");
  EXPECT_FALSE(parser_.Parse(&bad_stream4, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream5(&lexer_, "{\"a\": false");
  EXPECT_FALSE(parser_.Parse(&bad_stream5, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}

TEST(JsonParserTest, ParseArray) {
  TokenStream stream1(&lexer_, "[]");
  EXPECT_TRUE(parser_.Parse(&stream1, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(root_->children().size(), 0);

  TokenStream stream2(&lexer_, "[1, false, \"test\"]");
  EXPECT_TRUE(parser_.Parse(&stream2, root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(root_->children().size(), 3);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(JsonLexer::TYPE_FALSE));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
}

TEST(JsonParserTest, ParseArrayError) {
  TokenStream bad_stream1(&lexer_, "[, false");
  EXPECT_FALSE(parser_.Parse(&bad_stream1, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream2(&lexer_, "[1, false, null");
  EXPECT_FALSE(parser_.Parse(&bad_stream2, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}

TEST(JsonParserTest, ParseMultipleExpressions) {
  TokenStream stream(&lexer_, "1 false");
  EXPECT_FALSE(parser_.Parse(&stream, root_.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}
