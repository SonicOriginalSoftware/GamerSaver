#include "oauth.h"
#include "oauthtest.h"
#include "testresults.h"
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>

int GSTest::OAuthTest::SetUp() const { return 0; }

//bool GSTest::OAuthTest::Login(const GS::OAuth2 &oauth) const
//{
  //QNetworkAccessManager qnam{};
//#ifdef SHOULD_FAIL
  //return oauth.RequestLogin(qnam) == GS::OAuth2::UNKNOWN;
//#else
  //return oauth.RequestLogin(qnam) == GS::OAuth2::LOGGED_IN;
//#endif
//}

int main(int argc, char *argv[]) {
  GSTest::OAuthTest oauthTest{};
  if (oauthTest.SetUp() != 0) return TestResult::FAILED_SETUP;

  GS::OAuth2 oauth{};
  if (oauth.Errored()) return TestResult::FAIL;

  int testResults{0};

  // Test the Login procedure
  //testResults += oauthTest.Login(oauth) ? TestResult::PASS : TestResult::FAIL;

  return testResults;
}

