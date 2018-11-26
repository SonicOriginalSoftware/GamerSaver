#pragma once

namespace GS {
class OAuth2;
}

namespace GSTest {
enum TestResult {
  PASS = 0,
  FAIL = 1,
  SKIPPED = 77,
  FAILED_SETUP = 99,
  FAILED_TEARDOWN = 100
};

class GSTest {
protected:
  virtual TestResult SetUp() const;
  virtual TestResult TearDown() const;
  virtual TestResult Test() = 0;

public:
  virtual ~GSTest() = default;
  TestResult RunTest();
};
} // namespace GSTest
