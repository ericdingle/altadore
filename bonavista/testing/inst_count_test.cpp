#include "bonavista/testing/inst_count.h"

#include "bonavista/testing/test_case.h"

namespace {

class Dummy : public testing::InstCount<Dummy> {
public:
  Dummy() : testing::InstCount<Dummy>() {}
  ~Dummy() {}
};

}  // namespace

TEST_CASE(InstanceCountTest) {
};

TEST(InstanceCountTest, ConstructorAndDestructor) {
  EXPECT_EQ(Dummy::inst_count(), 0);
  {
    Dummy count;
    EXPECT_EQ(Dummy::inst_count(), 1);
  }
  EXPECT_EQ(Dummy::inst_count(), 0);
}
