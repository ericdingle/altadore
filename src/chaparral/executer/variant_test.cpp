#include "chaparral/executer/variant.h"

#include "bonavista/memory/scoped_ptr.h"
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

class Dummy {
};

TEST(VariantTest, GetObject) {
  memory::scoped_ptr<Dummy> dummy(new Dummy());
  Dummy* dummy1 = dummy.ptr();

  Variant var(dummy.Release());

  Dummy* dummy2;
  EXPECT_TRUE(var.Get(&dummy2));
  EXPECT_EQ(dummy1, dummy2);
}
