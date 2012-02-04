#include "chaparral/executer/variant.h"

#include "bonavista/memory/ref_count.h"
#include "bonavista/memory/scoped_refptr.h"
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

class Dummy : public memory::RefCount {
};

TEST(VariantTest, GetObject) {
  memory::scoped_refptr<Dummy> dummy(new Dummy());
  EXPECT_EQ(1, dummy->ref_count());

  Variant var(dummy);
  EXPECT_EQ(2, dummy->ref_count());

  memory::scoped_refptr<Dummy> dummy2;
  EXPECT_TRUE(var.Get(dummy2.Receive()));
  EXPECT_EQ(3, dummy->ref_count());
  EXPECT_EQ(dummy.ptr(), dummy2.ptr());
}
