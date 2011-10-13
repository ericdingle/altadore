#include "bonavista/memory/scoped_ref.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"
#include "bonavista/memory/ref_count.h"

namespace {

class Dummy : public memory::RefCount,
              public testing::InstCount<Dummy> {
public:
  Dummy() : RefCount(), testing::InstCount<Dummy>() {}
  ~Dummy() {}
};

}

using memory::scoped_ref;

TEST_CASE(ScopedRefTest) {
};

TEST(ScopedRefTest, ConstructorAndDestructor) {
  EXPECT_EQ(Dummy::inst_count(), 0);
  {
    scoped_ref<Dummy> dummy(new Dummy());
    EXPECT_EQ(dummy->ref_count(), 1);
    EXPECT_EQ(Dummy::inst_count(), 1);

    {
      scoped_ref<Dummy>dummy2(dummy);
      EXPECT_EQ(dummy->ref_count(), 2);
      EXPECT_EQ(Dummy::inst_count(), 1);
    }

    EXPECT_EQ(dummy->ref_count(), 1);
    EXPECT_EQ(Dummy::inst_count(), 1);
  }
  EXPECT_EQ(Dummy::inst_count(), 0);
}

TEST(ScopedRefTest, ConstCopyConstructor) {
  scoped_ref<Dummy> dummy1;
  scoped_ref<const Dummy> dummy2(dummy1);
  EXPECT_EQ(Dummy::inst_count(), 0);

  scoped_ref<Dummy> dummy3(new Dummy());
  EXPECT_EQ(dummy3->ref_count(), 1);
  EXPECT_EQ(Dummy::inst_count(), 1);

  scoped_ref<const Dummy> dummy4(dummy3);
  EXPECT_EQ(dummy3->ref_count(), 2);
  EXPECT_EQ(Dummy::inst_count(), 1);
}

TEST(ScopedRefTest, AssignmentOperator) {
  scoped_ref<Dummy> dummy1;
  scoped_ref<Dummy> dummy2;
  dummy2 = dummy1;
  EXPECT_EQ(Dummy::inst_count(), 0);

  scoped_ref<Dummy> dummy3(new Dummy());
  EXPECT_EQ(dummy3->ref_count(), 1);
  EXPECT_EQ(Dummy::inst_count(), 1);

  scoped_ref<Dummy> dummy4;
  dummy4 = dummy3;
  EXPECT_EQ(dummy3->ref_count(), 2);
  EXPECT_EQ(Dummy::inst_count(), 1);

  dummy3 = dummy1;
  EXPECT_EQ(dummy4->ref_count(), 1);
  EXPECT_EQ(Dummy::inst_count(), 1);

  dummy4 = dummy1;
  EXPECT_EQ(Dummy::inst_count(), 0);
}

TEST(ScopedRefTest, Vector) {
  std::vector<scoped_ref<Dummy> > dummies;

  scoped_ref<Dummy> dummy1(new Dummy());
  EXPECT_EQ(dummy1->ref_count(), 1);

  dummies.push_back(dummy1);
  EXPECT_EQ(dummy1->ref_count(), 2);

  scoped_ref<Dummy> dummy2(new Dummy());
  EXPECT_EQ(dummy2->ref_count(), 1);

  dummies.push_back(scoped_ref<Dummy>(dummy2));
  EXPECT_EQ(dummy1->ref_count(), 2);
  EXPECT_EQ(dummy2->ref_count(), 2);
}
