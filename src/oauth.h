#pragma once
#include <QString>

namespace GS {
enum ReturnCodes {
  OK,
  NETWORK_ERR,
  CANCELLED,
  UNHANDLED,
  SERVER_ERR,
  IO_ERR,
  CONSENT_DENIED,
  NON_UNIQUE_REQUEST,
  CONSENT_ERR
};

class OAuth2 {
protected:
  struct OAuthEndpoints {
    static const QString discoveryDoc;
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
    static const QString TokenResponseType;
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

  const QString& GetProfileName() const;
  const QString& GetProfilePictureURL() const;
  const QString& GetDiscoveryDocEndpoint() const;
  const QString& GetAuthEndpoint() const;
  const QString& GetUserInfoEndpoint() const;
  void SetAuthEndpoint(const QString&);
  void SetUserInfoEndpoint(const QString&);

  const QByteArray& GetAccessToken() const;
  QString GetRedirectUri() const;

  const QString BuildURL(int) const;

  ReturnCodes HandleConsent(const QByteArray&);
  void SetUser(const QByteArray&);
  void LogOut();
};
} // namespace GS
