#include "gstest.h"
#include "oauth.h"
#include "appmanifest.h"

namespace GSTest {
class TEST_PopulateGoogleEndPoints : public GSTest {
  GS::OAuth2 oauth{GS::AppManifest::ClientID, GS::AppManifest::RedirectURI};

  TestResult SetUp() const override;

  TestResult Test() override {
    return TestResult::PASS;
  }
};

TestResult TEST_PopulateGoogleEndPoints::SetUp() const {
  return TestResult::PASS;
}
} // namespace GSTest

int main(int argc, char *argv[]) {
  GSTest::TEST_PopulateGoogleEndPoints test{};
  return test.RunTest();
}
