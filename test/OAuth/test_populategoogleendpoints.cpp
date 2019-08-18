#include "gstest.h"
#include "oauth.h"

namespace GSTest
{
class TEST_PopulateGoogleEndPoints : public GSTest
{
  GS::OAuth2 oauth{};

  const TestResult SetUp() const override;

  const TestResult GSTest::Test()
  {
    return GS::ReturnCodes::OK == oauth.PopulateGoogleEndpoints() ?
      TestResult::PASS : TestResult::FAIL;
  }
};

const TestResult TEST_PopulateGoogleEndPoints::SetUp() const
{
  if (oauth.Errored()) return TestResult::FAILED_SETUP;
}
}

int main(int argc, char *argv[])
{
  GSTest::TEST_PopulateGoogleEndPoints test{};
  return test.RunTest();
}
