#pragma once
#include <QString>

namespace OAuth {
class OAuth2 {
protected:
  struct OAuthParameters {
    static const QString ResponseType;
    static const QString Scope;
    static const QString Prompt;
    static const QString State;
    static const QString ClientId;
    static const QString RedirectUri;
  };

  struct OAuthEndpoints {
    QString auth;
    QString userInfo;
  } endpoints;

  struct UserProfile {
    QString pictureURL;
    QString name;
  } profile;

  struct Tokens {
    QString accessToken;
  } tokens;

  void ClearUser();
};
} // namespace OAuth
