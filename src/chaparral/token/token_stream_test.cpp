#include "chaparral/token/token_stream.h"

#include "bonavista/testing/test_case.h"

using memory::scoped_ptr;

TEST_CASE(TokenStreamTest) {
};

TEST(TokenStreamTest, GetNextTokenEmpty) {
  TokenStream token_stream("");
  scoped_ptr<const Token> token;
  EXPECT_FALSE(token_stream.GetNextToken(token.Receive()));
}

TEST(TokenStreamTest, GetNextTokenIdentifier) {
  TokenStream token_stream("erad_94");
  scoped_ptr<const Token> token;
  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::IDENTIFIER));
  EXPECT_EQ(token->value(), "erad_94");
  EXPECT_FALSE(token_stream.GetNextToken(token.Receive()));
}

TEST(TokenStreamTest, GetNextTokenNumber) {
  const char* true_test_cases[] = {
    "0",
    "09",
    "10",
    "1e2",
    "5E4",
    "3e+3",
    "2E-5",
    "0.1",
    "09.1",
    "0.1e0",
    "0.1E0",
    "0.1e+0",
    "0.1E+0",
    "0.1e-0",
    "0.1E-0",
    "0.1e004",
    "0.1E004"
  };

  for (uint i = 0; i < 17; ++ i) {
    const char* test_case = true_test_cases[i];
    TokenStream token_stream(test_case);
    scoped_ptr<const Token> token;
    EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
    EXPECT_TRUE(token->IsType(Token::NUMBER));
    EXPECT_EQ(token->value(), test_case);
  }

  const char* false_test_cases[] = {
    "0.",
    "0.e",
    "0.E",
    "0.4e",
    "0.4E",
    "0.4e+",
    "0.4E+",
    "0.4e-",
  };

  for (uint i = 0; i < 8; ++i) {
    const char* test_case = false_test_cases[i];
    TokenStream token_stream(test_case);
    scoped_ptr<const Token> token;
    EXPECT_FALSE(token_stream.GetNextToken(token.Receive()));
  }
}

TEST(TokenStreamTest, GetNextTokenString) {
  const char* true_test_cases[][2] = {
    {"'asdf'", "'asdf'"},
    {"\"asdf\"", "\"asdf\""},
    {"'as\"df'", "'as\"df'"},
    {"\"as'df\"", "\"as'df\""},
    {"'as\\'df'", "'as'df'"},
    {"\"as\\\"df\"", "\"as\"df\""},
    {"'as\\\\df'", "'as\\df'"},
    {"'as\\ndf'", "'as\ndf'"},
    {"'as\\bdf'", "'as\bdf'"},
    {"'as\\tdf'", "'as\tdf'"},
  };

  for (int i = 0; i < 10; ++i) {
    const char** test_case = true_test_cases[i];
    TokenStream token_stream(test_case[0]);
    scoped_ptr<const Token> token;
    EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
    EXPECT_TRUE(token->IsType(Token::STRING));
    EXPECT_EQ(token->value(), test_case[1]);
  }

  const char* false_test_cases[] = {
    "'asdf",
    "\"asdf",
    "'asdf\"",
    "\"asdf'",
  };

  for (uint i = 0; i < 4; ++i) {
    const char* test_case = false_test_cases[i];
    TokenStream token_stream(test_case);
    scoped_ptr<const Token> token;
    EXPECT_FALSE(token_stream.GetNextToken(token.Receive()));
  }
}

TEST(TokenStreamTest, GetNextTokenOperator) {
  TokenStream token_stream("+");
  scoped_ptr<const Token> token;
  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::OPERATOR));
  EXPECT_EQ(token->value(), "+");
}

TEST(TokenStreamTest, GetNextToken) {
  TokenStream token_stream("rad r4 ear9e0\n1234 4.321 = ; 'asdf' \"test\"");
  scoped_ptr<const Token> token;

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::IDENTIFIER));
  EXPECT_EQ(token->value(), "rad");
  EXPECT_EQ(token->location().line, 1);
  EXPECT_EQ(token->location().column, 1);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::IDENTIFIER));
  EXPECT_EQ(token->value(), "r4");
  EXPECT_EQ(token->location().line, 1);
  EXPECT_EQ(token->location().column, 5);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::IDENTIFIER));
  EXPECT_EQ(token->value(), "ear9e0");
  EXPECT_EQ(token->location().line, 1);
  EXPECT_EQ(token->location().column, 8);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::NUMBER));
  EXPECT_EQ(token->value(), "1234");
  EXPECT_EQ(token->location().line, 2);
  EXPECT_EQ(token->location().column, 1);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::NUMBER));
  EXPECT_EQ(token->value(), "4.321");
  EXPECT_EQ(token->location().line, 2);
  EXPECT_EQ(token->location().column, 6);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::OPERATOR));
  EXPECT_EQ(token->value(), "=");
  EXPECT_EQ(token->location().line, 2);
  EXPECT_EQ(token->location().column, 12);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::OPERATOR));
  EXPECT_EQ(token->value(), ";");
  EXPECT_EQ(token->location().line, 2);
  EXPECT_EQ(token->location().column, 14);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::STRING));
  EXPECT_EQ(token->value(), "'asdf'");
  EXPECT_EQ(token->location().line, 2);
  EXPECT_EQ(token->location().column, 16);

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token->IsType(Token::STRING));
  EXPECT_EQ(token->value(), "\"test\"");
  EXPECT_EQ(token->location().line, 2);
  EXPECT_EQ(token->location().column, 23);

  EXPECT_FALSE(token_stream.GetNextToken(token.Receive()));
}

TEST(TokenStreamTest, PeekNextToken) {
  TokenStream token_stream("5 4");
  const Token* peek_token;
  scoped_ptr<const Token> token;

  EXPECT_TRUE(token_stream.PeekNextToken(&peek_token));
  EXPECT_EQ("5", peek_token->value());

  EXPECT_TRUE(token_stream.PeekNextToken(&peek_token));
  EXPECT_EQ("5", peek_token->value());

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_EQ("5", token->value());

  EXPECT_TRUE(token_stream.PeekNextToken(&peek_token));
  EXPECT_EQ("4", peek_token->value());

  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_EQ("4", token->value());

  EXPECT_FALSE(token_stream.PeekNextToken(&peek_token));
}

TEST(TokenStreamTest, EndOfInput) {
  TokenStream token_stream("5");
  const Token* peek_token;
  scoped_ptr<const Token> token;
  EXPECT_FALSE(token_stream.EndOfInput());
  EXPECT_TRUE(token_stream.PeekNextToken(&peek_token));
  EXPECT_FALSE(token_stream.EndOfInput());
  EXPECT_TRUE(token_stream.GetNextToken(token.Receive()));
  EXPECT_TRUE(token_stream.EndOfInput());
}
