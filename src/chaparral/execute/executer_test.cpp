#include "chaparral/execute/executer.h"

#include "bonavista/testing/test_case.h"
#include "chaparral/execute/expression.h"
#include "chaparral/parse/identifier_symbol.h"
#include "chaparral/parse/number_symbol.h"
#include "chaparral/parse/string_symbol.h"
#include "chaparral/value/number_value.h"
#include "chaparral/value/string_value.h"

namespace {

class TestExpression : public Expression, public Symbol {
public:
  TestExpression(bool b) : Symbol(new Token(Token::OPERATOR, "+", 1, 1)), b_(b) {}

  bool Execute(Executer* executer, memory::scoped_ref<Value>* result) const {
    if (b_)
      *result = new NumberValue(5);
    return b_;
  }

private:
  bool b_;
};

}  // namespace

TEST_CASE(ExecuterTest) {
public:
  void SetUp() {
    result_.Reset(NULL);
  }

protected:
  Executer executer_;
  memory::scoped_ref<Value> result_;
};

TEST(ExecuterTest, ExecuteIdentifier) {
  IdentifierSymbol identifier_symbol(new Token(Token::IDENTIFIER, "a", 1, 1));
  EXPECT_TRUE(executer_.Execute(&identifier_symbol, &result_));
  EXPECT_NULL(result_.ptr());
}

TEST(ExecuterTest, ExecuteNumber) {
  NumberSymbol number_symbol(new Token(Token::NUMBER, "5", 1, 1));
  EXPECT_TRUE(executer_.Execute(&number_symbol, &result_));
  NumberValue* number_value = dynamic_cast<NumberValue*>(result_.ptr());
  EXPECT_NOT_NULL(number_value);
  EXPECT_EQ(number_value->Get(), 5);
}

TEST(ExecuterTest, ExecuteString) {
  StringSymbol string_symbol(new Token(Token::STRING, "asdf", 1, 1));
  EXPECT_TRUE(executer_.Execute(&string_symbol, &result_));
  StringValue* string_value = dynamic_cast<StringValue*>(result_.ptr());
  EXPECT_NOT_NULL(string_value);
  EXPECT_EQ(string_value->Get(), "asdf");
}

TEST(ExecuterTest, ExecuteSymbol) {
  Symbol symbol(new Token(Token::OPERATOR, "+", 1, 1));
  EXPECT_FALSE(executer_.Execute(&symbol, &result_));
}

TEST(ExecuterTest, ExecuteExpressionTrue) {
  TestExpression expression(true);
  EXPECT_TRUE(executer_.Execute(&expression, &result_));
  EXPECT_NOT_NULL(result_.ptr());
}

TEST(ExecuterTest, ExecuteExpressionFalse) {
  TestExpression expression(false);
  EXPECT_FALSE(executer_.Execute(&expression, &result_));
  EXPECT_NULL(result_.ptr());
}
