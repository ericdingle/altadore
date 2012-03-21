#include "bonavista/file/util.h"

#include <string.h>
#include "bonavista/file/scoped_file.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(UtilTest) {
 protected:
  void TearDown() {
    remove("test.tmp");
  }
};

TEST(UtilTest, OpenFile) {
  scoped_FILE file(OpenFile("test.tmp", "w"));
  EXPECT_NOT_NULL(file.ptr());
}

TEST(UtilTest, ReadFile) {
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

TEST(UtilTest, ReadFileError) {
  scoped_FILE file(OpenFile("test.tmp", "r"));
  EXPECT_NULL(file.ptr());
}
