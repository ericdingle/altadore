#include "chaparral/json/json_parser.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/json/json_lexer.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(JsonParserTest) {
 protected:
  JsonLexer lexer_;
  JsonParser parser_;
};

TEST(JsonParserTest, ParseMultipleExpressions) {
  memory::scoped_ptr<const ASTNode> root;
  TokenStream stream(&lexer_, "1 false");
  EXPECT_FALSE(parser_.Parse(&stream, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}

TEST(JsonParserTest, ParseObject) {
  memory::scoped_ptr<const ASTNode> root;

  TokenStream stream1(&lexer_, "{}");
  EXPECT_TRUE(parser_.Parse(&stream1, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(root->children().size(), 0);

  TokenStream stream2(&lexer_, "{\"a\": 1, \"b\": false}");
  EXPECT_TRUE(parser_.Parse(&stream2, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(root->children().size(), 4);

  EXPECT_TRUE(root->children()[0]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root->children()[1]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root->children()[3]->token()->IsType(JsonLexer::TYPE_FALSE));
}

TEST(JsonParserTest, ParseObjectError) {
  memory::scoped_ptr<const ASTNode> root;

  TokenStream bad_stream1(&lexer_, "{, \"a\": 1}");
  EXPECT_FALSE(parser_.Parse(&bad_stream1, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream2(&lexer_, "{1: false}");
  EXPECT_FALSE(parser_.Parse(&bad_stream2, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream3(&lexer_, "{\"a\", false}");
  EXPECT_FALSE(parser_.Parse(&bad_stream3, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream4(&lexer_, "{\"a\": }");
  EXPECT_FALSE(parser_.Parse(&bad_stream4, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream5(&lexer_, "{\"a\": false");
  EXPECT_FALSE(parser_.Parse(&bad_stream5, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}

TEST(JsonParserTest, ParseArray) {
  memory::scoped_ptr<const ASTNode> root;

  TokenStream stream1(&lexer_, "[]");
  EXPECT_TRUE(parser_.Parse(&stream1, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(root->children().size(), 0);

  TokenStream stream2(&lexer_, "[1, false, \"test\"]");
  EXPECT_TRUE(parser_.Parse(&stream2, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(root->children().size(), 3);

  EXPECT_TRUE(root->children()[0]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root->children()[1]->token()->IsType(JsonLexer::TYPE_FALSE));
  EXPECT_TRUE(root->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
}

TEST(JsonParserTest, ParseArrayError) {
  memory::scoped_ptr<const ASTNode> root;

  TokenStream bad_stream1(&lexer_, "[, false");
  EXPECT_FALSE(parser_.Parse(&bad_stream1, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());

  TokenStream bad_stream2(&lexer_, "[1, false, null");
  EXPECT_FALSE(parser_.Parse(&bad_stream2, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}

TEST(JsonParserTest, ParsePrimitive) {
  memory::scoped_ptr<const ASTNode> root;

  TokenStream stream1(&lexer_, "false");
  EXPECT_TRUE(parser_.Parse(&stream1, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_FALSE));

  TokenStream stream2(&lexer_, "null");
  EXPECT_TRUE(parser_.Parse(&stream2, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_NULL));

  TokenStream stream3(&lexer_, "1");
  EXPECT_TRUE(parser_.Parse(&stream3, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_NUMBER));

  TokenStream stream4(&lexer_, "\"test\"");
  EXPECT_TRUE(parser_.Parse(&stream4, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_STRING));

  TokenStream stream5(&lexer_, "true");
  EXPECT_TRUE(parser_.Parse(&stream5, root.Receive()));
  EXPECT_TRUE(root->token()->IsType(JsonLexer::TYPE_TRUE));
}

TEST(JsonParserTest, ParsePrimitiveError) {
  memory::scoped_ptr<const ASTNode> root;

  TokenStream stream(&lexer_, "blah");
  EXPECT_FALSE(parser_.Parse(&stream, root.Receive()));
  EXPECT_FALSE(parser_.error().empty());
}
