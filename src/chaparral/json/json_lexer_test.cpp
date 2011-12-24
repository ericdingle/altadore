#include "chaparral/json/json_lexer.h"

#include "bonavista/util/macros.h"
#include "bonavista/testing/test_case.h"


TEST_CASE(JsonLexerTest) {
};

TEST(JsonLexerTest, Operators) {
  JsonLexer lexer;

  std::string input = "";
  const uint index = 0;
  int type = -1;
  std::string value;
  uint count = 0;
  std::string error;

  const char* inputs[] = { ":", ",", "{", "}", "[", "]" };
  const int types[] = { JsonLexer::TYPE_COLON,
                        JsonLexer::TYPE_COMMA,
                        JsonLexer::TYPE_LEFT_BRACE,
                        JsonLexer::TYPE_RIGHT_BRACE,
                        JsonLexer::TYPE_LEFT_BRACKET,
                        JsonLexer::TYPE_RIGHT_BRACKET };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input = inputs[i];
    EXPECT_TRUE(lexer.GetToken(input, index, &type, &value, &count, &error));
    EXPECT_EQ(type, types[i]);
    EXPECT_EQ(value, input);
    EXPECT_EQ(count, 1);
  }

  input = ".";
  EXPECT_FALSE(lexer.GetToken(input, index, &type, &value, &count, &error));
  EXPECT_FALSE(error.empty());
}

TEST(JsonLexerTest, Keywords) {
  JsonLexer lexer;

  std::string input = "";
  const uint index = 0;
  int type = -1;
  std::string value;
  uint count = 0;
  std::string error;

  const char* inputs[] = { "false", "null", "true" };
  const int types[] = { JsonLexer::TYPE_FALSE,
                        JsonLexer::TYPE_NULL,
                        JsonLexer::TYPE_TRUE };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input = inputs[i];
    EXPECT_TRUE(lexer.GetToken(input, index, &type, &value, &count, &error));
    EXPECT_EQ(type, types[i]);
    EXPECT_EQ(value, input);
    EXPECT_EQ(count, input.length());
  }

  input = "blah";
  EXPECT_FALSE(lexer.GetToken(input, index, &type, &value, &count, &error));
  EXPECT_FALSE(error.empty());
}

TEST(JsonLexerTest, Numbers) {
  JsonLexer lexer;

  std::string input = "";
  const uint index = 0;
  int type = -1;
  std::string value;
  uint count = 0;
  std::string error;

  const char* inputs[] = {
    "0", "-0", "1", "-1", "12", "123",
    "0.1", "-0.1", "12.3", "12.34",
    "5e3", "32E-1", "120e+012"
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input = inputs[i];
    EXPECT_TRUE(lexer.GetToken(input, index, &type, &value, &count, &error));
    EXPECT_EQ(type, JsonLexer::TYPE_NUMBER);
    EXPECT_EQ(value, input);
    EXPECT_EQ(count, input.length());
  }

  const char* bad_inputs[] = { "-", "01", "1.", "23e", "35E+" };

  for (uint i = 0; i < ARRAY_SIZE(bad_inputs); ++i) {
    input = bad_inputs[i];
    error.clear();
    EXPECT_FALSE(lexer.GetToken(input, index, &type, &value, &count, &error));
    EXPECT_FALSE(error.empty());
  }
}

TEST(JsonLexerTest, Strings) {
  JsonLexer lexer;

  std::string input = "";
  const uint index = 0;
  int type = -1;
  std::string value;
  uint count = 0;
  std::string error;

  const char* inputs[] = {
    "\"test\"",
    "\"asdf jkl;\"",
    "\"test\\b\"",
    "\"test\\f\"",
    "\"test\\n\"",
    "\"test\\r\"",
    "\"test\\t\"",
    "\"test\\u1234\"",
    "\"test\\\\\"",
    "\"test\\/\"",
    "\"test\\\"\"",
  };
  const char* values[] = {
    "test",
    "asdf jkl;",
    "test\b",
    "test\f",
    "test\n",
    "test\r",
    "test\t",
    "test?",
    "test\\",
    "test/",
    "test\"",
  };

  for (uint i = 0; i < ARRAY_SIZE(inputs); ++i) {
    input = inputs[i];
    EXPECT_TRUE(lexer.GetToken(input, index, &type, &value, &count, &error));
    EXPECT_EQ(type, JsonLexer::TYPE_STRING);
    EXPECT_EQ(value, values[i]);
    EXPECT_EQ(count, input.length());
  }

  const char* bad_inputs[] = {
    "\"test",
    "\"test\n",
    "\"test\\",
    "\"test\\a",
    "\"test\\u12",
    "\"test\\u123e"
  };

  for (uint i = 0; i < ARRAY_SIZE(bad_inputs); ++i) {
    input = bad_inputs[i];
    error.clear();
    EXPECT_FALSE(lexer.GetToken(input, index, &type, &value, &count, &error));
    EXPECT_FALSE(error.empty());
  }
}
