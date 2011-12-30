#include "chaparral/json/json_parser.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/json/json_lexer.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(JsonParserTest) {
 protected:
  JsonLexer lexer_;
  memory::scoped_ptr<const ASTNode> root_;
};

TEST(JsonParserTest, ParseEmpty) {
  TokenStream stream(&lexer_, "");
  JsonParser parser(&stream);
  EXPECT_FALSE(parser.Parse(root_.Receive()));
  EXPECT_FALSE(parser.error().empty());
}

TEST(JsonParserTest, ParseUnknown) {
  TokenStream stream(&lexer_, "blah");
  JsonParser parser(&stream);
  EXPECT_FALSE(parser.Parse(root_.Receive()));
  EXPECT_FALSE(parser.error().empty());
}

TEST(JsonParserTest, ParsePrimitive) {
  TokenStream stream1(&lexer_, "false");
  JsonParser parser1(&stream1);
  EXPECT_TRUE(parser1.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_FALSE));

  TokenStream stream2(&lexer_, "null");
  JsonParser parser2(&stream2);
  EXPECT_TRUE(parser2.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_NULL));

  TokenStream stream3(&lexer_, "1");
  JsonParser parser3(&stream3);
  EXPECT_TRUE(parser3.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_NUMBER));

  TokenStream stream4(&lexer_, "\"test\"");
  JsonParser parser4(&stream4);
  EXPECT_TRUE(parser4.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_STRING));

  TokenStream stream5(&lexer_, "true");
  JsonParser parser5(&stream5);
  EXPECT_TRUE(parser5.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_TRUE));
}

TEST(JsonParserTest, ParseObject) {
  TokenStream stream1(&lexer_, "{}");
  JsonParser parser1(&stream1);
  EXPECT_TRUE(parser1.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(root_->children().size(), 0);

  TokenStream stream2(&lexer_, "{\"a\": 1, \"b\": false}");
  JsonParser parser2(&stream2);
  EXPECT_TRUE(parser2.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(root_->children().size(), 4);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root_->children()[3]->token()->IsType(JsonLexer::TYPE_FALSE));
}

TEST(JsonParserTest, ParseObjectError) {
  TokenStream stream1(&lexer_, "{, \"a\": 1}");
  JsonParser parser1(&stream1);
  EXPECT_FALSE(parser1.Parse(root_.Receive()));
  EXPECT_FALSE(parser1.error().empty());

  TokenStream stream2(&lexer_, "{1: false}");
  JsonParser parser2(&stream2);
  EXPECT_FALSE(parser2.Parse(root_.Receive()));
  EXPECT_FALSE(parser2.error().empty());

  TokenStream stream3(&lexer_, "{\"a\", false}");
  JsonParser parser3(&stream3);
  EXPECT_FALSE(parser3.Parse(root_.Receive()));
  EXPECT_FALSE(parser3.error().empty());

  TokenStream stream4(&lexer_, "{\"a\": }");
  JsonParser parser4(&stream4);
  EXPECT_FALSE(parser4.Parse(root_.Receive()));
  EXPECT_FALSE(parser4.error().empty());

  TokenStream stream5(&lexer_, "{\"a\": false");
  JsonParser parser5(&stream5);
  EXPECT_FALSE(parser5.Parse(root_.Receive()));
  EXPECT_FALSE(parser5.error().empty());
}

TEST(JsonParserTest, ParseArray) {
  TokenStream stream1(&lexer_, "[]");
  JsonParser parser1(&stream1);
  EXPECT_TRUE(parser1.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(root_->children().size(), 0);

  TokenStream stream2(&lexer_, "[1, false, \"test\"]");
  JsonParser parser2(&stream2);
  EXPECT_TRUE(parser2.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(root_->children().size(), 3);

  EXPECT_TRUE(root_->children()[0]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(JsonLexer::TYPE_FALSE));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
}

TEST(JsonParserTest, ParseArrayError) {
  TokenStream stream1(&lexer_, "[, false");
  JsonParser parser1(&stream1);
  EXPECT_FALSE(parser1.Parse(root_.Receive()));
  EXPECT_FALSE(parser1.error().empty());

  TokenStream stream2(&lexer_, "[1, false, null");
  JsonParser parser2(&stream2);
  EXPECT_FALSE(parser2.Parse(root_.Receive()));
  EXPECT_FALSE(parser2.error().empty());
}

TEST(JsonParserTest, ParseMultipleExpressions) {
  TokenStream stream1(&lexer_, "1 false");
  JsonParser parser1(&stream1);
  EXPECT_FALSE(parser1.Parse(root_.Receive()));
  EXPECT_FALSE(parser1.error().empty());

  TokenStream stream2(&lexer_, "1 blah");
  JsonParser parser2(&stream2);
  EXPECT_FALSE(parser2.Parse(root_.Receive()));
  EXPECT_FALSE(parser2.error().empty());
}
