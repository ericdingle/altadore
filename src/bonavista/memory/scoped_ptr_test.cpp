#include "bonavista/memory/scoped_ptr.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"

struct AddFunc {
  void operator()(int* i) const {
    if (i)
      (*i)++;
  }
};

struct SubFunc {
  void operator()(int* i) const {
    if (i)
      (*i)--;
  }
};

typedef scoped_ptr_base<int, AddFunc, SubFunc> TestScopedPtrBase;

TEST_CASE(ScopedPtrBaseTest) {
};

TEST(ScopedPtrBaseTest, ConstructorAndDestructor) {
  int i = 0;
  {
    TestScopedPtrBase ptr(&i);
    EXPECT_EQ(i, 1);
  }
  EXPECT_EQ(i, 0);
}


TEST(ScopedPtrBaseTest, Get) {
  TestScopedPtrBase ptr1;
  EXPECT_NULL(ptr1.ptr());

  int i = 0;
  TestScopedPtrBase ptr2(&i);
  EXPECT_EQ(ptr2.ptr(), &i);
}

TEST(ScopedPtrBaseTest, Reset) {
  int i1 = 0;
  TestScopedPtrBase ptr(&i1);
  EXPECT_EQ(i1, 1);

  int i2 = 0;
  ptr.Reset(&i2);
  EXPECT_EQ(i1, 0);
  EXPECT_EQ(i2, 1);
}

TEST(ScopedPtrBaseTest, Release) {
  int i = 0;
  TestScopedPtrBase ptr(&i);
  int* ip = ptr.Release();
  EXPECT_NULL(ptr.ptr());
  EXPECT_EQ(&i, ip);
}

TEST(ScopedPtrBaseTest, Receive) {
  int i = 0;
  TestScopedPtrBase ptr;
  int** ipp = ptr.Receive();
  *ipp = &i;
  *ptr.ptr() = 1;
  EXPECT_EQ(1, i);
}

TEST_CASE(ScopedPtrTest) {
};

TEST(ScopedPtrTest, ConstructorAndDestructor) {
  EXPECT_EQ(InstCount::inst_count(), 0);
  {
    scoped_ptr<InstCount> ptr(new InstCount());
    EXPECT_EQ(InstCount::inst_count(), 1);
  }
  EXPECT_EQ(InstCount::inst_count(), 0);
}
