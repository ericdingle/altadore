#include "bonavista/memory/scoped_ref.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"
#include "bonavista/memory/ref_count.h"

namespace {

class Dummy : public memory::RefCount,
              public testing::InstCount {
};

}  // namespace

TEST_CASE(ScopedRefTest) {
};

TEST(ScopedRefTest, ConstructorAndDestructor) {
  EXPECT_EQ(Dummy::inst_count(), 0);
  {
    memory::scoped_ref<Dummy> ref(new Dummy());
    EXPECT_EQ(ref->ref_count(), 1);
    EXPECT_EQ(Dummy::inst_count(), 1);

    {
      memory::scoped_ref<Dummy>ref2(ref);
      EXPECT_EQ(ref->ref_count(), 2);
      EXPECT_EQ(Dummy::inst_count(), 1);
    }

    EXPECT_EQ(ref->ref_count(), 1);
    EXPECT_EQ(Dummy::inst_count(), 1);
  }
  EXPECT_EQ(Dummy::inst_count(), 0);
}

TEST(ScopedRefTest, ConstCopyConstructor) {
  memory::scoped_ref<Dummy> ref1;
  memory::scoped_ref<const Dummy> ref2(ref1);
  EXPECT_EQ(Dummy::inst_count(), 0);

  memory::scoped_ref<Dummy> ref3(new Dummy());
  EXPECT_EQ(ref3->ref_count(), 1);
  EXPECT_EQ(Dummy::inst_count(), 1);

  memory::scoped_ref<const Dummy> ref4(ref3);
  EXPECT_EQ(ref3->ref_count(), 2);
  EXPECT_EQ(Dummy::inst_count(), 1);
}

TEST(ScopedRefTest, AssignmentOperator) {
  memory::scoped_ref<Dummy> ref1;
  memory::scoped_ref<Dummy> ref2;
  ref2 = ref1;
  EXPECT_EQ(Dummy::inst_count(), 0);

  memory::scoped_ref<Dummy> ref3(new Dummy());
  EXPECT_EQ(ref3->ref_count(), 1);
  EXPECT_EQ(Dummy::inst_count(), 1);

  memory::scoped_ref<Dummy> ref4;
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
  std::vector<memory::scoped_ref<Dummy> > dummies;

  memory::scoped_ref<Dummy> ref1(new Dummy());
  EXPECT_EQ(ref1->ref_count(), 1);

  dummies.push_back(ref1);
  EXPECT_EQ(ref1->ref_count(), 2);

  memory::scoped_ref<Dummy> ref2(new Dummy());
  EXPECT_EQ(ref2->ref_count(), 1);

  dummies.push_back(memory::scoped_ref<Dummy>(ref2));
  EXPECT_EQ(ref1->ref_count(), 2);
  EXPECT_EQ(ref2->ref_count(), 2);
}
