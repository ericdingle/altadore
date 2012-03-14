#include "bonavista/file/util.h"

#include <string.h>
#include "bonavista/file/scoped_file.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(IoTest) {
 protected:
  void TearDown() {
    remove("test.tmp");
  }
};

TEST(IoTest, OpenFile) {
  scoped_FILE file(OpenFile("test.tmp", "w"));
  EXPECT_NOT_NULL(file.ptr());
}

TEST(IoTest, ReadFile) {
  {
    scoped_FILE file(OpenFile("test.tmp", "w"));
    EXPECT_NOT_NULL(file.ptr());
    const char* str = "test";
    EXPECT_EQ(fwrite(str, 1, strlen(str), file.ptr()), strlen(str));
  }

  std::string contents;
  EXPECT_TRUE(ReadFile("test.tmp", &contents));
  EXPECT_EQ("test", contents);
}
