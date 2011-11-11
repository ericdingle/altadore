#include "bonavista/testing/test_case.h"

#include <stdio.h>
#include <string.h>
#include "bonavista/logging/color_printf.h"

#ifdef OS_WIN
#define SIZE_T_FORMAT "%Iu"
#else
#define SIZE_T_FORMAT "%zu"
#endif

namespace {

const char* kDivider = "[------------]";
const char* kRun =     "[ Run        ]";
const char* kPass =    "[       Pass ]";
const char* kFail =    "[       Fail ]";
const char* kPassed =  "[   Passed   ]";
const char* kFailed =  "[   Failed   ]";

}  // namespace

namespace testing {

uint TestCase::RunAll() {
  uint num_test_cases = 0;
  std::vector<TestCase*> failures;

  std::vector<TestCase*>& tests = GetTests();
  for (uint i = 0; i < tests.size();) {
    ++num_test_cases;
    uint num_tests = GetNumTests(i);
    ColorPrintf(logging::COLOR_GREEN, kDivider);
    printf(" %d test(s) from %s\n", num_tests, tests[i]->testCaseName_);

    for (uint j = 0; j < num_tests; ++i, ++j)
      if (!RunTest(tests[i]))
        failures.push_back(tests[i]);

    ColorPrintf(logging::COLOR_GREEN, kDivider);
    printf(" %d test(s) from %s\n\n", num_tests, tests[i-1]->testCaseName_);
  }

  ColorPrintf(logging::COLOR_GREEN, kDivider);
  printf("\n");

  if (failures.size() == 0) {
    ColorPrintf(logging::COLOR_GREEN, kPassed);
    printf(" " SIZE_T_FORMAT " test(s) from %u test case(s)\n", tests.size(), num_test_cases);
  } else {
    ColorPrintf(logging::COLOR_RED, kFailed);
    printf(" " SIZE_T_FORMAT " test(s)\n", failures.size());

    for (uint i = 0; i < failures.size(); ++i) {
      ColorPrintf(logging::COLOR_RED, kDivider);
      printf(" %s.%s\n", tests[i]->testCaseName_, tests[i]->testName_);
    }
  }

  ColorPrintf(logging::COLOR_GREEN, kDivider);
  printf("\n");

  return failures.size();
}

void TestCase::AddTest(TestCase* test) {
  GetTests().push_back(test);
}

TestCase::TestCase() {
}

TestCase::TestCase(const char* testCaseName, const char* testName) : testCaseName_(testCaseName), testName_(testName), succeeded_(true) {
}

TestCase::~TestCase() {
}

std::vector<TestCase*>& TestCase::GetTests() {
  static std::vector<TestCase*> tests;
  return tests;
}

uint TestCase::GetNumTests(uint index) {
  std::vector<TestCase*>& tests = GetTests();
  for (uint end = index + 1; end < tests.size(); ++end)
    if (strcmp(tests[end]->testCaseName_, tests[index]->testCaseName_) != 0)
      return end - index;
  return tests.size() - index;
}

bool TestCase::RunTest(TestCase* test) {
  ColorPrintf(logging::COLOR_GREEN, kRun);
  printf(" %s.%s\n", test->testCaseName_, test->testName_);

  test->SetUp();

  if (test->succeeded_) {
    test->Run();
  }

  test->TearDown();

  if (test->succeeded_) {
    ColorPrintf(logging::COLOR_GREEN, kPass);
  } else {
    ColorPrintf(logging::COLOR_RED, kFail);
  }

  printf(" %s.%s\n", test->testCaseName_, test->testName_);
  return test->succeeded_;
}

void TestCase::SetUp() {
}

void TestCase::TearDown() {
}

}  // namespace testing
