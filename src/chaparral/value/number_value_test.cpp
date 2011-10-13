#include "chaparral/value/number_value.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(NumberValueTest) {
};

TEST(NumberValueTest, Constructor) {
  NumberValue value;
  EXPECT_EQ(value.Get(), 0.0);
}

TEST(NumberValueTest, Get) {
  NumberValue value(5.0);
  EXPECT_EQ(value.Get(), 5.0);
}

TEST(NumberValueTest, ToString) {
  NumberValue value(5);
  EXPECT_EQ(value.ToString(), "5");

  NumberValue value2(5.5);
  EXPECT_EQ(value2.ToString(), "5.5");

  NumberValue value3(3.14159);
  EXPECT_EQ(value3.ToString(), "3.14159");
}
