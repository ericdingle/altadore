#include "chaparral/value/string_value.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(StringValueTest) {
};

TEST(StringValueTest, Constructor) {
  StringValue value;
  EXPECT_EQ(value.Get(), "");
}

TEST(StringValueTest, Get) {
  StringValue value("asdf");
  EXPECT_EQ(value.Get(), "asdf");
}

TEST(StringValueTest, ToString) {
  StringValue value("asdf");
  EXPECT_EQ(value.ToString(), "\"asdf\"");
}
