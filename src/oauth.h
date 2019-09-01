#pragma once
#include <QString>

namespace GS {
enum class OAuthReturnCodes {
  OK,
  CONSENT_ERR,
  CONSENT_DENIED,
  NON_UNIQUE_REQUEST
};

class OAuth2 {
protected:
  struct OAuthEndpoints {
    QString auth;
    QString userInfo;
  };

  struct UserProfile {
    QString pictureURL;
    QString name;
  };

  struct Tokens {
    QByteArray accessToken;
  };

  struct OAuthParameters {
    static const QString ResponseType;
    static const QString Scope;
    static const QString Prompt;
    static const QString State;
  };

  const QString client_id;
  const QString redirect_uri;
  OAuthEndpoints endpoints;

private:
  const QString profilePictureFileName;

  UserProfile profile;
  Tokens tokens;

public:
  explicit OAuth2(const QString&, const QString&);

  void LogOut();
  QString GetProfileName() const;
  QString GetProfilePictureURL() const;
  QString GetAuthEndpoint() const;
  QString GetUserInfoEndpoint() const;
  QString GetRedirectUri() const;
  QByteArray GetAccessToken() const;

  void SetAuthEndpoint(const QString&);
  void SetUserInfoEndpoint(const QString&);
  void SetUser(const QByteArray&);

  QString BuildURL(int) const;
  OAuthReturnCodes HandleConsent(const QByteArray&);
};
} // namespace GS
