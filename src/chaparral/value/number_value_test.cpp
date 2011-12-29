#include "chaparral/value/number_value.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(NumberValueTest) {
};

TEST(NumberValueTest, GetAsNumber) {
  NumberValue value(5.0);
  double d;
  EXPECT_TRUE(value.GetAsNumber(&d));
  EXPECT_EQ(d, 5.0);
}
