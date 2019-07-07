#include <QString>
#include <QStringList>
#include "testresults.h"
#include "oauthtest.h"
#include "oauth.h"

int GSTest::OAuthTest::SetUp() const
{
  return 0;
}

bool GSTest::OAuthTest::ClientIDCorrect(const GS::OAuth2 &oauth) const
{
#ifdef SHOULD_FAIL
  return *oauth.credentials.client_id == "none";
#else
  return *oauth.credentials.client_id == "207822922610-ocssudnmrb7guhri57s565afv1ldb4er.apps.googleusercontent.com";
#endif
}

int main(int argc, char *argv[])
{

  GSTest::OAuthTest oauthTest{};
  if (oauthTest.SetUp() != 0)
    return TestResult::FAILED_SETUP;

  GS::OAuth2 oauth{};
  if (oauth.Errored())
    return TestResult::FAIL;
  return oauthTest.ClientIDCorrect(oauth) ? TestResult::PASS : TestResult::FAIL;
}
