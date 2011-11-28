#include "chaparral/lexer/token_stream.h"

#include "bonavista/memory/scoped_ptr.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/lexer.h"

TEST_CASE(TokenStreamTest) {
};

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_CHAR
  };

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const {
    DASSERT(type != NULL);
    DASSERT(value != NULL);
    DASSERT(count != NULL);
    DASSERT(error != NULL);

    *type = TYPE_CHAR;
    *value = input[index];
    *count = 1;
    return true;
  }
};

TEST(TokenStreamTest, GetNextToken) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "a bc\nde  ");

  memory::scoped_ptr<const Token> token;
  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "a");
  EXPECT_EQ(token->position().line, 1);
  EXPECT_EQ(token->position().column, 1);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "b");
  EXPECT_EQ(token->position().line, 1);
  EXPECT_EQ(token->position().column, 3);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "c");
  EXPECT_EQ(token->position().line, 1);
  EXPECT_EQ(token->position().column, 4);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "d");
  EXPECT_EQ(token->position().line, 2);
  EXPECT_EQ(token->position().column, 1);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ(token->value(), "e");
  EXPECT_EQ(token->position().line, 2);
  EXPECT_EQ(token->position().column, 2);

  EXPECT_FALSE(token_stream.GetNextToken(token.Receive()));
  EXPECT_FALSE(token_stream.error().empty());
}

TEST(TokenStreamTest, PeekNextToken) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "ab");

  const Token* peek_token;
  EXPECT_TRUE(token_stream.PeekNextToken(&peek_token));
  EXPECT_EQ("a", peek_token->value());

  EXPECT_TRUE(token_stream.PeekNextToken(&peek_token));
  EXPECT_EQ("a", peek_token->value());

  memory::scoped_ptr<const Token> token;
  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_EQ("a", token->value());

  EXPECT_TRUE(token_stream.PeekNextToken(&peek_token));
  EXPECT_EQ("b", peek_token->value());

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_EQ("b", token->value());

  EXPECT_FALSE(token_stream.PeekNextToken(&peek_token));
}

TEST(TokenStreamTest, EndOfInput) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "a");
  EXPECT_FALSE(token_stream.EndOfInput());

  const Token* peek_token;
  EXPECT_TRUE(token_stream.PeekNextToken(&peek_token));
  EXPECT_FALSE(token_stream.EndOfInput());

  memory::scoped_ptr<const Token> token;
  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token_stream.EndOfInput());
}
