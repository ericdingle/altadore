#include "chaparral/calc/calc_parser.h"

#include "bonavista/string/format.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/calc/calc_lexer.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(CalcParserTest) {
 protected:
  CalcLexer lexer_;
  memory::scoped_ptr<const ASTNode> root_;
};

TEST(CalcParserTest, ParseEmpty) {
  TokenStream stream(&lexer_, "");
  CalcParser parser(&stream);
  EXPECT_FALSE(parser.Parse(root_.Receive()));
  EXPECT_FALSE(parser.error().empty());
}

TEST(CalcParserTest, ParseUnknown) {
  TokenStream stream(&lexer_, "b");
  CalcParser parser(&stream);
  EXPECT_FALSE(parser.Parse(root_.Receive()));
  EXPECT_FALSE(parser.error().empty());
}

TEST(CalcParserTest, ParseNumber) {
  TokenStream stream(&lexer_, "1");
  CalcParser parser(&stream);
  EXPECT_TRUE(parser.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(CalcLexer::TYPE_NUMBER));
}

TEST(CalcParserTest, ParseParenthesis) {
  TokenStream stream(&lexer_, "(1)");
  CalcParser parser(&stream);
  EXPECT_TRUE(parser.Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(CalcLexer::TYPE_NUMBER));
}

TEST(CalcParserTest, ParseParenthesisError) {
  const char* inputs[] = { "()", "(1", "(+)" };
  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    TokenStream stream(&lexer_, inputs[i]);
    CalcParser parser(&stream);
    EXPECT_FALSE(parser.Parse(root_.Receive()));
    EXPECT_FALSE(parser.error().empty());
  }
}

TEST(CalcParserTest, ParseOperator) {
  const char ops[] = { '*', '-', '+', '/' };
  const int types[] = { CalcLexer::TYPE_ASTERISK,
                        CalcLexer::TYPE_MINUS,
                        CalcLexer::TYPE_PLUS,
                        CalcLexer::TYPE_SLASH };

  for (uint i = 0; i < ARRAY_SIZE(ops); ++i) {
    std::string input = string::Format("1%c2", ops[i]);
    TokenStream stream(&lexer_, input);
    CalcParser parser(&stream);
    EXPECT_TRUE(parser.Parse(root_.Receive()));

    EXPECT_EQ(root_->token()->type(), types[i]);
    EXPECT_EQ(root_->children().size(), 2);

    EXPECT_EQ(root_->children()[0]->token()->type(), CalcLexer::TYPE_NUMBER);
    EXPECT_EQ(root_->children()[0]->token()->value(), "1");

    EXPECT_EQ(root_->children()[1]->token()->type(), CalcLexer::TYPE_NUMBER);
    EXPECT_EQ(root_->children()[1]->token()->value(), "2");
  }
}

TEST(CalcParserTest, ParseOperatorError) {
  TokenStream stream(&lexer_, "1+*");
  CalcParser parser(&stream);
  EXPECT_FALSE(parser.Parse(root_.Receive()));
  EXPECT_FALSE(parser.error().empty());
}

TEST(CalcParserTest, OperatorPrecedence) {
  const char* inputs[] = { "1+2-3",
                           "1-2+3",
                           "1*2/3",
                           "1/2*3",
                           "1*2+3",
                           "1-2/3",
                           "1*(2+3)" };
  const int types[] = { CalcLexer::TYPE_MINUS,
                        CalcLexer::TYPE_PLUS,
                        CalcLexer::TYPE_SLASH,
                        CalcLexer::TYPE_ASTERISK,
                        CalcLexer::TYPE_PLUS,
                        CalcLexer::TYPE_MINUS,
                        CalcLexer::TYPE_ASTERISK };
  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    TokenStream stream(&lexer_, inputs[i]);
    CalcParser parser(&stream);
    EXPECT_TRUE(parser.Parse(root_.Receive()));
    EXPECT_EQ(root_->token()->type(), types[i]);
  }
}

TEST(CalcParserTest, ParseMultipleExpressions) {
  TokenStream stream1(&lexer_, "1 2");
  CalcParser parser1(&stream1);
  EXPECT_FALSE(parser1.Parse(root_.Receive()));
  EXPECT_FALSE(parser1.error().empty());

  TokenStream stream2(&lexer_, "1 a");
  CalcParser parser2(&stream2);
  EXPECT_FALSE(parser2.Parse(root_.Receive()));
  EXPECT_FALSE(parser2.error().empty());
}
