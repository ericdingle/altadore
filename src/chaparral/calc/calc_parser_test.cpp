#include "chaparral/calc/calc_parser.h"

#include "bonavista/string/format.h"
#include "bonavista/testing/test_case.h"
#include "chaparral/calc/calc_lexer.h"
#include "chaparral/lexer/token_stream.h"
#include "chaparral/parser/ast_node.h"

TEST_CASE(CalcParserTest) {
 protected:
  void Init(const char* input) {
    stream_.Reset(new TokenStream(&lexer_, input));
    parser_.Reset(new CalcParser(stream_.ptr()));
  }

  CalcLexer lexer_;
  memory::scoped_ptr<TokenStream> stream_;
  memory::scoped_ptr<Parser> parser_;
  memory::scoped_ptr<const ASTNode> root_;
};

TEST(CalcParserTest, ParseEmpty) {
  Init("");
  EXPECT_FALSE(parser_->Parse(root_.Receive()));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(CalcParserTest, ParseMultiple) {
  Init("1 2");
  EXPECT_FALSE(parser_->Parse(root_.Receive()));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(CalcParserTest, ParseUnknown) {
  Init("b");
  EXPECT_FALSE(parser_->Parse(root_.Receive()));
  EXPECT_FALSE(parser_->error().empty());
}

TEST(CalcParserTest, ParseNumber) {
  Init("1");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(CalcLexer::TYPE_NUMBER));
}

TEST(CalcParserTest, ParseParenthesis) {
  Init("(1)");
  EXPECT_TRUE(parser_->Parse(root_.Receive()));
  EXPECT_TRUE(root_->token()->IsType(CalcLexer::TYPE_NUMBER));
}

TEST(CalcParserTest, ParseParenthesisError) {
  const char* inputs[] = { "()", "(1", "(+)" };
  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    Init(inputs[i]);
    EXPECT_FALSE(parser_->Parse(root_.Receive()));
    EXPECT_FALSE(parser_->error().empty());
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
    Init(input.c_str());
    EXPECT_TRUE(parser_->Parse(root_.Receive()));

    EXPECT_EQ(root_->token()->type(), types[i]);
    EXPECT_EQ(root_->children().size(), 2);

    EXPECT_EQ(root_->children()[0]->token()->type(), CalcLexer::TYPE_NUMBER);
    EXPECT_EQ(root_->children()[0]->token()->value(), "1");

    EXPECT_EQ(root_->children()[1]->token()->type(), CalcLexer::TYPE_NUMBER);
    EXPECT_EQ(root_->children()[1]->token()->value(), "2");
  }
}

TEST(CalcParserTest, ParseOperatorError) {
  Init("1+*");
  EXPECT_FALSE(parser_->Parse(root_.Receive()));
  EXPECT_FALSE(parser_->error().empty());
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
    Init(inputs[i]);
    EXPECT_TRUE(parser_->Parse(root_.Receive()));
    EXPECT_EQ(root_->token()->type(), types[i]);
  }
}
