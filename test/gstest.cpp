#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include "GSTest.h"

namespace GSTest
{
const TestResult GSTest::SetUp() const { }

const TestResult GSTest::TearDown() const { return TestResult::PASS; }

const TestResult GSTest::RunTest()
{
  if (SetUp() != TestResult::PASS) return TestResult::FAILED_SETUP;
  TestResult testResult{Test()};
  if (TearDown() != TestResult::PASS) return TestResult::FAILED_TEARDOWN;
  return testResult;
}
}
