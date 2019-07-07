#include "oauthtest.h"
#include "oauth.h"
#include "testresults.h"
#include <QString>
#include <QStringList>

int GSTest::OAuthTest::SetUp() const { return 0; }

bool GSTest::OAuthTest::ClientIDCorrect(const GS::OAuth2 &oauth) const {
#ifdef SHOULD_FAIL
  return oauth.ClientID() == "none";
#else
  return oauth.ClientID() == "207822922610-ocssudnmrb7guhri57s565afv1ldb4er.apps.googleusercontent.com";
#endif
}

bool GSTest::OAuthTest::AuthURICorrect(const GS::OAuth2 &oauth) const {
#ifdef SHOULD_FAIL
  return oauth.AuthURI() == "none";
#else
  return oauth.AuthURI() == "https://accounts.google.com/o/oauth2/auth";
#endif
}

bool GSTest::OAuthTest::TokenURICorrect(const GS::OAuth2 &oauth) const {
#ifdef SHOULD_FAIL
  return oauth.TokenURI() == "none";
#else
  return oauth.TokenURI() == "https://oauth2.googleapis.com/token";
#endif
}

bool GSTest::OAuthTest::RedirectURIsCorrect(const GS::OAuth2 &oauth) const {
#ifdef SHOULD_FAIL
  return oauth.RedirectURIs() == QStringList{};
#else
  return oauth.RedirectURIs() == QStringList{"urn:ietf:wg:oauth:2.0:oob", "http://localhost"};
#endif
}

int main(int argc, char *argv[]) {
  GSTest::OAuthTest oauthTest{};
  if (oauthTest.SetUp() != 0) return TestResult::FAILED_SETUP;

  GS::OAuth2 oauth{};
  if (oauth.Errored()) return TestResult::FAIL;

  int testResults{0};
  // Test the credentials struct
  testResults += oauthTest.ClientIDCorrect(oauth) ? TestResult::PASS : TestResult::FAIL;
  testResults += oauthTest.AuthURICorrect(oauth) ? TestResult::PASS : TestResult::FAIL;
  testResults += oauthTest.TokenURICorrect(oauth) ? TestResult::PASS : TestResult::FAIL;
  testResults += oauthTest.RedirectURIsCorrect(oauth) ? TestResult::PASS : TestResult::FAIL;

  return testResults;
}

