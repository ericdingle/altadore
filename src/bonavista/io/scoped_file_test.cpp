#include "bonavista/io/scoped_file.h"

#include "bonavista/io/io.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(ScopedFileTest) {
protected:
  void SetUp() {
    file_ = io::OpenFile("test.tmp", "w+b");
  }

  void TearDown() {
    remove("test.tmp");
  }

  FILE* file_;
};

TEST(ScopedFileTest, ConstructorAndDestructor) {
  {
    io::scoped_FILE scoped_file(file_);

    int bytes[] = {1, 2, 3, 4};
    EXPECT_EQ(fwrite(bytes, sizeof(int), 4, file_), 4);

    EXPECT_EQ(fseek(file_, 0, SEEK_SET), 0);
    int buffer[4];
    EXPECT_EQ(fread(&buffer, sizeof(int), 4, file_), 4);

    for (int i = 0; i < 4; ++i) {
      EXPECT_EQ(buffer[i], bytes[i]);
    };
  }

  EXPECT_NOT_EQ(fseek(file_, 0, SEEK_SET), 0);
}
