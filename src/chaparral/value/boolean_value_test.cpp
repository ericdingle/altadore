#include "chaparral/value/boolean_value.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(BooleanValueTest) {
};

TEST(BooleanValueTest, GetAsNumber) {
  BooleanValue value(true);
  bool b;
  EXPECT_TRUE(value.GetAsBoolean(&b));
  EXPECT_TRUE(b);
}
