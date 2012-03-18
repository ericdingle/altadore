#include "bonavista/memory/scoped_refptr.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"
#include "bonavista/memory/ref_counted.h"

namespace {

class Dummy : public RefCounted, public InstCount {
};

}  // namespace

TEST_CASE(ScopedRefTest) {
};

TEST(ScopedRefTest, ConstructorAndDestructor) {
  EXPECT_EQ(Dummy::inst_count(), 0);
  {
    scoped_refptr<Dummy> ref(new Dummy());
    EXPECT_EQ(ref->ref_count(), 1);
    EXPECT_EQ(Dummy::inst_count(), 1);

    {
      scoped_refptr<Dummy>ref2(ref);
      EXPECT_EQ(ref->ref_count(), 2);
      EXPECT_EQ(Dummy::inst_count(), 1);
    }

    EXPECT_EQ(ref->ref_count(), 1);
    EXPECT_EQ(Dummy::inst_count(), 1);
  }
  EXPECT_EQ(Dummy::inst_count(), 0);
}

TEST(ScopedRefTest, ConstCopyConstructor) {
  scoped_refptr<Dummy> ref1;
  scoped_refptr<const Dummy> ref2(ref1);
  EXPECT_EQ(Dummy::inst_count(), 0);

  scoped_refptr<Dummy> ref3(new Dummy());
  EXPECT_EQ(ref3->ref_count(), 1);
  EXPECT_EQ(Dummy::inst_count(), 1);

  scoped_refptr<const Dummy> ref4(ref3);
  EXPECT_EQ(ref3->ref_count(), 2);
  EXPECT_EQ(Dummy::inst_count(), 1);
}

TEST(ScopedRefTest, AssignmentOperator) {
  scoped_refptr<Dummy> ref1;
  scoped_refptr<Dummy> ref2;
  ref2 = ref1;
  EXPECT_EQ(Dummy::inst_count(), 0);

  scoped_refptr<Dummy> ref3(new Dummy());
  EXPECT_EQ(ref3->ref_count(), 1);
  EXPECT_EQ(Dummy::inst_count(), 1);

  scoped_refptr<Dummy> ref4;
  ref4 = ref3;
  EXPECT_EQ(ref3->ref_count(), 2);
  EXPECT_EQ(Dummy::inst_count(), 1);

  ref3 = ref1;
  EXPECT_EQ(ref4->ref_count(), 1);
  EXPECT_EQ(Dummy::inst_count(), 1);

  ref4 = ref1;
  EXPECT_EQ(Dummy::inst_count(), 0);
}

TEST(ScopedRefTest, Vector) {
  std::vector<scoped_refptr<Dummy> > dummies;

  scoped_refptr<Dummy> ref1(new Dummy());
  EXPECT_EQ(ref1->ref_count(), 1);

  dummies.push_back(ref1);
  EXPECT_EQ(ref1->ref_count(), 2);

  scoped_refptr<Dummy> ref2(new Dummy());
  EXPECT_EQ(ref2->ref_count(), 1);

  dummies.push_back(scoped_refptr<Dummy>(ref2));
  EXPECT_EQ(ref1->ref_count(), 2);
  EXPECT_EQ(ref2->ref_count(), 2);
}
