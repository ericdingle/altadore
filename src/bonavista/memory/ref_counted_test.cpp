#include "bonavista/memory/ref_counted.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"

namespace {

class Dummy : public RefCounted, public InstCount {
};

}  // namespace

TEST_CASE(RefCountedTest) {
};

TEST(RefCountedTest, AddRefAndRelease) {
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
