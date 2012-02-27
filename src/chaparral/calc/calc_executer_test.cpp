#include "chaparral/calc/calc_executer.h"

#include "bonavista/string/format.h"
#include "bonavista/testing/test_case.h"
#include "bonavista/util/macros.h"
#include "chaparral/calc/calc_lexer.h"
#include "chaparral/calc/calc_parser.h"
#include "chaparral/lexer/token_stream.h"

TEST_CASE(CalcExecuterTest) {
 protected:
  void Init(const char* input) {
    stream_.Reset(new TokenStream(&lexer_, input));
    parser_.Reset(new CalcParser(stream_.ptr()));
    executer_.Reset(new CalcExecuter(parser_.ptr()));
  }

  CalcLexer lexer_;
  memory::scoped_ptr<TokenStream> stream_;
  memory::scoped_ptr<Parser> parser_;
  memory::scoped_ptr<Executer> executer_;
  memory::scoped_refptr<const Variant> var_;
  double d_;
};

TEST(CalcExecuterTest, ExecuteEmpty) {
  Init("");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(CalcExecuterTest, ExecuteUnknown) {
  Init("b");
  EXPECT_FALSE(executer_->Execute(var_.Receive()));
  EXPECT_FALSE(executer_->error().empty());
}

TEST(CalcExecuterTest, ExecuteNumber) {
  Init("1");
  EXPECT_TRUE(executer_->ExecuteT(&d_));
  EXPECT_EQ(d_, 1);
}

TEST(CalcExecuterTest, ExecuteOperator) {
  const char ops[] = { '*', '-', '+', '/' };
  const double results[] = { 9, 0, 6, 1 };

  for (uint i = 0; i < ARRAY_SIZE(ops); ++i) {
    std::string input = string::Format("3%c3", ops[i]);
    Init(input.c_str());
    EXPECT_TRUE(executer_->ExecuteT(&d_));
    EXPECT_EQ(d_, results[i]);
  }
}
