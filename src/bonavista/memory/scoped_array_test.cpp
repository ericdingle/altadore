#include "bonavista/memory/scoped_array.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(ScopedArrayTest) {
};

TEST(ScopedArrayTest, ConstructorAndDestructor) {
  EXPECT_EQ(InstCount::inst_count(), 0);
  {
    memory::scoped_array<InstCount> array(new InstCount[5]);
    EXPECT_EQ(InstCount::inst_count(), 5);
  }
  EXPECT_EQ(InstCount::inst_count(), 0);
}

TEST(ScopedArrayTest, IndexOperator) {
  memory::scoped_array<int> array(new int[5]);
  array[2] = 10;
  EXPECT_EQ(array[2], 10);
  EXPECT_EQ(array.ptr()[2], 10);
}

TEST(ScopedArrayTest, ConstIndexOperator) {
  int* i = new int[5];
  i[2] = 10;

  const memory::scoped_array<int> array(i);
  EXPECT_EQ(array[2], 10);
}
