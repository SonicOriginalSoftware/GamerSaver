#include "gstest.h"
#include <QNetworkAccessManager>
#include <QString>
#include <QStringList>

namespace GSTest {
TestResult GSTest::SetUp() const { return TestResult::PASS; }

TestResult GSTest::TearDown() const { return TestResult::PASS; }

TestResult GSTest::RunTest() {
  if (SetUp() != TestResult::PASS) return TestResult::FAILED_SETUP;
  TestResult testResult{Test()};
  if (TearDown() != TestResult::PASS) return TestResult::FAILED_TEARDOWN;
  return testResult;
}
} // namespace GSTest
