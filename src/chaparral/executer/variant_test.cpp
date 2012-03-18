#include "chaparral/executer/variant.h"

#include "bonavista/memory/ref_counted.h"
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

class Dummy : public RefCounted {
};

class Dummy2 : public RefCounted {
};

TEST(VariantTest, GetObject) {
  scoped_refptr<Dummy> dummy(new Dummy());
  EXPECT_EQ(1, dummy->ref_count());

  Variant var(dummy.ptr());
  EXPECT_EQ(2, dummy->ref_count());

  scoped_refptr<Dummy> dummy1;
  EXPECT_TRUE(var.Get(dummy1.Receive()));
  EXPECT_EQ(dummy.ptr(), dummy1.ptr());
  EXPECT_EQ(3, dummy->ref_count());

  scoped_refptr<Dummy2> dummy2;
  EXPECT_FALSE(var.Get(dummy2.Receive()));
}
