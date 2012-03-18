#include "bonavista/base/command_line.h"

#include "bonavista/testing/test_case.h"

const char* args[] = {
  "prog",
  "arg1",
  "--input=file1.txt",
  "--output=",
  "--enable-logging",
  "-not-a-flag",
  "arg2"
};

TEST_CASE(CommandLineTest) {
};

TEST(CommandLineTest, Parse) {
  CommandLine cmd_line(ARRAY_SIZE(args), args);

  EXPECT_EQ( cmd_line.program(), "prog");

  const std::vector<std::string>& args = cmd_line.args();
  EXPECT_EQ(args.size(), 3);
  EXPECT_EQ(args[0], "arg1");
  EXPECT_EQ(args[1], "-not-a-flag");
  EXPECT_EQ(args[2], "arg2");

  EXPECT_TRUE(cmd_line.HasFlag("input"));
  EXPECT_EQ(cmd_line.GetFlag("input"), "file1.txt");
  EXPECT_TRUE(cmd_line.HasFlag("output"));
  EXPECT_EQ(cmd_line.GetFlag("output"), "");
  EXPECT_TRUE(cmd_line.HasFlag("enable-logging"));
  EXPECT_EQ(cmd_line.GetFlag("enable-logging"), "");
}
