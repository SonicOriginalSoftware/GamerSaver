#pragma once

namespace GS { class OAuth2; }

namespace GSTest
{
enum TestResult { PASS = 0, FAIL = 1, SKIPPED = 77, FAILED_SETUP = 99, FAILED_TEARDOWN = 100 };

class GSTest {
protected:
  virtual const TestResult SetUp() const;
  virtual const TestResult TearDown() const;
  virtual const TestResult Test() = 0;
public:
  const TestResult RunTest();
};
}
