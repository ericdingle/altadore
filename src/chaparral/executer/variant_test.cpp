#include "chaparral/executer/variant.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(VariantTest) {
};

TEST(VariantTest, GetPrimitive) {
  Variant var(5);

  int i = 0;
  EXPECT_TRUE(var.Get(&i));
  EXPECT_EQ(i, 5);

  double d;
  EXPECT_FALSE(var.Get(&d));
}

TEST(VariantTest, GetObject) {
  testing::InstCount inst;
  Variant var(&inst);

  testing::InstCount* inst_ptr = NULL;
  EXPECT_TRUE(var.Get(&inst_ptr));
  EXPECT_EQ(&inst, inst_ptr);
}

TEST(VariantTest, GetConstObject) {
  const testing::InstCount inst;
  Variant var(&inst);

  const testing::InstCount* inst_ptr = NULL;
  EXPECT_TRUE(var.Get(&inst_ptr));
  EXPECT_EQ(&inst, inst_ptr);
}
