#pragma once
class QNetworkAccessManager;

namespace GS {
class OAuth2;
}

namespace GSTest
{
class OAuthTest {

public:
  QNetworkAccessManager *qnam{nullptr};

  explicit OAuthTest();
  ~OAuthTest();

  int SetUp() const;
  bool ClientIDCorrect(const GS::OAuth2& oauth) const;
  bool AuthURICorrect(const GS::OAuth2& oauth) const;
  bool TokenURICorrect(const GS::OAuth2& oauth) const;
  bool RedirectURIsCorrect(const GS::OAuth2& oauth) const;
  bool Login(const QNetworkAccessManager&, const GS::OAuth2& oauth) const;
};
}

