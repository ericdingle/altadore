#include "bonavista/memory/scoped_array.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(ScopedArrayTest) {
};

TEST(ScopedArrayTest, ConstructorAndDestructor) {
  EXPECT_EQ(testing::InstCount::inst_count(), 0);
  {
    memory::scoped_array<testing::InstCount> array(new testing::InstCount[5]);
    EXPECT_EQ(testing::InstCount::inst_count(), 5);
  }
  EXPECT_EQ(testing::InstCount::inst_count(), 0);
}

TEST(ScopedArrayTest, IndexOperator) {
  memory::scoped_array<int> array(new int[5]);
  array[2] = 10;
  EXPECT_EQ(array[2], 10);
  EXPECT_EQ(array.ptr()[2], 10);
}
