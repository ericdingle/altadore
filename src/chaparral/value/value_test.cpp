#include "chaparral/value/value.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(ValueTest) {
};

TEST(ValueTest, GetAs) {
  Value value(Value::TYPE_NULL);

  bool b;
  EXPECT_FALSE(value.GetAsBoolean(&b));

  double d;
  EXPECT_FALSE(value.GetAsNumber(&d));

  std::string str;
  EXPECT_FALSE(value.GetAsString(&str));
}
