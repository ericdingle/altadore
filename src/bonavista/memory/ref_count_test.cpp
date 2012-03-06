#include "bonavista/memory/ref_count.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"

namespace {

class Dummy : public memory::RefCount,
              public testing::InstCount {
};

}  // namespace

TEST_CASE(RefCountTest) {
};

TEST(RefCountTest, AddRefAndRelease) {
  EXPECT_EQ(Dummy::inst_count(), 0);
  Dummy* dummy = new Dummy();
  EXPECT_EQ(Dummy::inst_count(), 1);

  EXPECT_EQ(dummy->ref_count(), 0);
  dummy->AddRef();
  EXPECT_EQ(dummy->ref_count(), 1);
  dummy->AddRef();
  EXPECT_EQ(dummy->ref_count(), 2);
  dummy->Release();
  EXPECT_EQ(dummy->ref_count(), 1);
  dummy->Release();

  EXPECT_EQ(Dummy::inst_count(), 0);
}
