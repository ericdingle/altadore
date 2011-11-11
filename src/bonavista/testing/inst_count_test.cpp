#include "bonavista/testing/inst_count.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(InstanceCountTest) {
};

TEST(InstanceCountTest, ConstructorAndDestructor) {
  EXPECT_EQ(testing::InstCount::inst_count(), 0);
  {
    testing::InstCount count;
    EXPECT_EQ(testing::InstCount::inst_count(), 1);
  }
  EXPECT_EQ(testing::InstCount::inst_count(), 0);
}
