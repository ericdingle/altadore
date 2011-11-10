#include "bonavista/io/io.h"

#include <string.h>
#include "bonavista/io/scoped_file.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(IoTest) {
protected:
  void TearDown() {
    remove("test.tmp");
  }
};

TEST(IoTest, OpenFile) {
  io::scoped_FILE file(io::OpenFile("test.tmp", "w"));
  EXPECT_NOT_NULL(file.ptr());
}

TEST(IoTest, ReadFile) {
  {
    io::scoped_FILE file(io::OpenFile("test.tmp", "w"));
    EXPECT_NOT_NULL(file.ptr());
    const char* str = "test";
    EXPECT_EQ(fwrite(str, 1, strlen(str), file.ptr()), strlen(str));
  }

  std::string contents;
  EXPECT_TRUE(io::ReadFile("test.tmp", &contents));
  EXPECT_EQ("test", contents);
}
