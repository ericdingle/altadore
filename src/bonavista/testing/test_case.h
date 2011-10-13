#ifndef BONAVISTA_TESTING_TEST_CASE_H_
#define BONAVISTA_TESTING_TEST_CASE_H_

#include <vector>
#include "bonavista/testing/test_case_expect.h"
#include "bonavista/types.h"

namespace testing {

class TestCase {
public:
  static uint RunAll();

protected:
  static void AddTest(TestCase* test);

  TestCase();
  TestCase(const char* testCaseName, const char* testName);
  virtual ~TestCase();

  void set_succeeded(bool succeeded) { succeeded_ = succeeded; }

private:
  static std::vector<TestCase*>& GetTests();
  static uint GetNumTests(uint index);
  static bool RunTest(TestCase* test);

  virtual void SetUp();
  virtual void TearDown();
  virtual void Run()=0;

  TestCase(const TestCase&);
  void operator=(const TestCase&);

  const char* testCaseName_;
  const char* testName_;
  bool succeeded_;
};

}  // namespace testing

#define TEST_CASE(testCaseName) \
  class testCaseName : public virtual testing::TestCase

#define TEST_CLASS_NAME(testCaseName, testName) testCaseName##_##testName##_Test

#define TEST(testCaseName, testName) \
  class TEST_CLASS_NAME(testCaseName, testName) : public testCaseName { \
  private: \
    TEST_CLASS_NAME(testCaseName, testName)() : testing::TestCase(#testCaseName, #testName) { \
      testing::TestCase::AddTest(this); \
    } \
    void Run(); \
    static TEST_CLASS_NAME(testCaseName, testName) instance_; \
  }; \
  TEST_CLASS_NAME(testCaseName, testName) TEST_CLASS_NAME(testCaseName, testName)::instance_; \
  void TEST_CLASS_NAME(testCaseName, testName)::Run()

#endif
