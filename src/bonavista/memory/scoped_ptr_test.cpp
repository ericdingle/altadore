#include "bonavista/memory/scoped_ptr.h"

#include "bonavista/testing/inst_count.h"
#include "bonavista/testing/test_case.h"

namespace {

class AddFunc {
public:
  void operator()(int* i) const {
    if (i)
      (*i)++;
  }
};

class SubFunc {
public:
  void operator()(int* i) const {
    if (i)
      (*i)--;
  }
};

typedef memory::scoped_ptr_base<int, AddFunc, SubFunc> TestScopedPtrBase;

}

TEST_CASE(ScopedPtrBase) {
};

TEST(ScopedPtrBase, ConstructorAndDestructor) {
  int i = 0;
  {
    TestScopedPtrBase ptr(&i);
    EXPECT_EQ(i, 1);
  }
  EXPECT_EQ(i, 0);
}


TEST(ScopedPtrBase, Get) {
  TestScopedPtrBase ptr1;
  EXPECT_NULL(ptr1.ptr());

  int i = 0;
  TestScopedPtrBase ptr2(&i);
  EXPECT_EQ(ptr2.ptr(), &i);
}

TEST(ScopedPtrBase, Reset) {
  int i1 = 0;
  TestScopedPtrBase ptr(&i1);
  EXPECT_EQ(i1, 1);

  int i2 = 0;
  ptr.Reset(&i2);
  EXPECT_EQ(i1, 0);
  EXPECT_EQ(i2, 1);
}

TEST(ScopedPtrBase, Release) {
  int i = 0;
  TestScopedPtrBase ptr(&i);
  int* ip = ptr.Release();
  EXPECT_NULL(ptr.ptr());
  EXPECT_EQ(&i, ip);
}

TEST(ScopedPtrBase, Receive) {
  int i = 0;
  TestScopedPtrBase ptr;
  int** ipp = ptr.Receive();
  *ipp = &i;
  *ptr.ptr() = 1;
  EXPECT_EQ(1, i);
}

namespace {

class Dummy : public testing::InstCount<Dummy> {
public:
  Dummy() : testing::InstCount<Dummy>() {}
  ~Dummy() {}
};

}

TEST_CASE(ScopedPtrTest) {
};

TEST(ScopedPtrTest, ConstructorAndDestructor) {
  EXPECT_EQ(Dummy::inst_count(), 0);
  {
    memory::scoped_ptr<Dummy> ptr(new Dummy());
    EXPECT_EQ(Dummy::inst_count(), 1);
  }
  EXPECT_EQ(Dummy::inst_count(), 0);
}

TEST_CASE(ScopedArrayTest) {
};

TEST(ScopedArrayTest, ConstructorAndDestructor) {
  EXPECT_EQ(Dummy::inst_count(), 0);
  {
    memory::scoped_array<Dummy> array(new Dummy[5]);
    EXPECT_EQ(Dummy::inst_count(), 5);
  }
  EXPECT_EQ(Dummy::inst_count(), 0);
}

TEST(ScopedArrayTest, IndexOperator) {
  memory::scoped_array<int> array(new int[5]);
  array[2] = 10;
  EXPECT_EQ(array.ptr()[2], 10);
}
