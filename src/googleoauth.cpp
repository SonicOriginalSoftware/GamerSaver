#include "googleoauth.h"

namespace GS {
const QString GoogleOAuth::discoveryDocUrl{"https://accounts.google.com/.well-known/openid-configuration"};
const QString GoogleOAuth::OAuthParameters::ResponseType{"token"};
const QString GoogleOAuth::OAuthParameters::Scope{"https://www.googleapis.com/auth/drive.file profile"};
const QString GoogleOAuth::OAuthParameters::Prompt{"select_account"};

const QString GoogleOAuth::authorizationEndpointKeyName{ "authorization_endpoint"};
const QString GoogleOAuth::userInfoEndpointKeyName{"userinfo_endpoint"};

QString GoogleOAuth::GetDiscoveryDocEndpoint() { return discoveryDocUrl; }
QString GoogleOAuth::GetAuthEndpointKeyName() const { return authorizationEndpointKeyName; }
QString GoogleOAuth::GetUserInfoEndpointKeyName() const { return userInfoEndpointKeyName; }
} // namespace GS
