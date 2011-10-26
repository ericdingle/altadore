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

const char* DIVIDER = "[------------]";
const char* RUN = "[ Run        ]";
const char* PASS = "[       Pass ]";
const char* FAIL = "[       Fail ]";
const char* PASSED = "[   Passed   ]";
const char* FAILED = "[   Failed   ]";

}  // namespace

namespace testing {

uint TestCase::RunAll() {
  uint num_test_cases = 0;
  std::vector<TestCase*> failures;

  std::vector<TestCase*>& tests = GetTests();
  for (uint i = 0; i < tests.size();) {
    ++num_test_cases;
    uint num_tests = GetNumTests(i);
    ColorPrintf(logging::GREEN, DIVIDER);
    printf(" %d test(s) from %s\n", num_tests, tests[i]->testCaseName_);

    for (uint j = 0; j < num_tests; ++i, ++j)
      if (!RunTest(tests[i]))
        failures.push_back(tests[i]);

    ColorPrintf(logging::GREEN, DIVIDER);
    printf(" %d test(s) from %s\n\n", num_tests, tests[i-1]->testCaseName_);
  }

  ColorPrintf(logging::GREEN, DIVIDER);
  printf("\n");

  if (failures.size() == 0) {
    ColorPrintf(logging::GREEN, PASSED);
    printf(" " SIZE_T_FORMAT " test(s) from %u test case(s)\n", tests.size(), num_test_cases);
  } else {
    ColorPrintf(logging::RED, FAILED);
    printf(" " SIZE_T_FORMAT " test(s)\n", failures.size());

    for (uint i = 0; i < failures.size(); ++i) {
      ColorPrintf(logging::RED, DIVIDER);
      printf(" %s.%s\n", tests[i]->testCaseName_, tests[i]->testName_);
    }
  }

  ColorPrintf(logging::GREEN, DIVIDER);
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
  ColorPrintf(logging::GREEN, RUN);
  printf(" %s.%s\n", test->testCaseName_, test->testName_);

  test->SetUp();

  if (test->succeeded_) {
    test->Run();
  }

  test->TearDown();

  if (test->succeeded_) {
    ColorPrintf(logging::GREEN, PASS);
  } else {
    ColorPrintf(logging::RED, FAIL);
  }

  printf(" %s.%s\n", test->testCaseName_, test->testName_);
  return test->succeeded_;
}

void TestCase::SetUp() {
}

void TestCase::TearDown() {
}

}  // namespace testing
