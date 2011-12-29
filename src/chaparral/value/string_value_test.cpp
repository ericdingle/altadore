#include "chaparral/value/string_value.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(StringValueTest) {
};

TEST(StringValueTest, GetAsString) {
  StringValue value("test");
  std::string str;
  EXPECT_TRUE(value.GetAsString(&str));
  EXPECT_EQ(str, "test");
}
