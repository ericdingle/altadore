#include "bonavista/testing/inst_count.h"

#include "bonavista/testing/test_case.h"

TEST_CASE(InstCountTest) {
};

TEST(InstCountTest, ConstructorAndDestructor) {
  EXPECT_EQ(InstCount::inst_count(), 0);
  {
    InstCount count;
    EXPECT_EQ(InstCount::inst_count(), 1);
  }
  EXPECT_EQ(InstCount::inst_count(), 0);
}

class Dummy1 : public InstCountT<Dummy1> {};
class Dummy2 : public InstCountT<Dummy2> {};

TEST(InstCountTest, TemplateConstructorAndDestructor) {
  EXPECT_EQ(Dummy1::inst_count(), 0);
  EXPECT_EQ(Dummy2::inst_count(), 0);
  {
    Dummy1 dummy1;
    EXPECT_EQ(Dummy1::inst_count(), 1);
    EXPECT_EQ(Dummy2::inst_count(), 0);
  }
  EXPECT_EQ(Dummy1::inst_count(), 0);
  EXPECT_EQ(Dummy2::inst_count(), 0);
}
