#include "googleoauth.h"
#include <QUrl>

namespace GS {
const QString GoogleOAuth::discoveryDocUrl{"https://accounts.google.com/.well-known/openid-configuration"};
const QString GoogleOAuth::OAuthParameters::ResponseType{"token"};
const QString GoogleOAuth::OAuthParameters::Scope{"https://www.googleapis.com/auth/drive.file profile"};
const QString GoogleOAuth::OAuthParameters::Prompt{"select_account"};

const QString GoogleOAuth::authorizationEndpointKeyName{"authorization_endpoint"};
const QString GoogleOAuth::userInfoEndpointKeyName{"userinfo_endpoint"};

GoogleOAuth::GoogleOAuth(const QString& clientId, const QString& redirectUri)
  : parameters(clientId.toUtf8(), redirectUri.toUtf8()) {}

QString GoogleOAuth::GetDiscoveryDocEndpoint() { return discoveryDocUrl; }
QString GoogleOAuth::GetAuthEndpointKeyName() { return authorizationEndpointKeyName; }
QString GoogleOAuth::GetUserInfoEndpointKeyName() { return userInfoEndpointKeyName; }

QString GoogleOAuth::BuildURL(int serverPort) const {
  return GetAuthEndpoint()
    + "?client_id=" + parameters.clientId
    + "&redirect_uri=" + parameters.redirectUri + ":" + QString::number(serverPort)
    + "&response_type=" + OAuthParameters::ResponseType
    + "&scope=" + QUrl::toPercentEncoding(OAuthParameters::Scope)
    + "&state=" + OAuthParameters::State
    + "&prompt=" + OAuthParameters::Prompt;
}
} // namespace GS
