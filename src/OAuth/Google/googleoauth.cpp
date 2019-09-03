#include "googleoauth.h"

namespace OAuth {
const QString GoogleOAuth::DiscoveryDocUrl{"https://accounts.google.com/.well-known/openid-configuration"};
const QString GoogleOAuth::AuthorizationEndpointKeyName{"authorization_endpoint"};
const QString GoogleOAuth::UserInfoEndpointKeyName{"userinfo_endpoint"};

const QString OAuth2::OAuthParameters::ResponseType{"token"};
const QString OAuth2::OAuthParameters::Prompt{"select_account"};

QString GoogleOAuth::BuildURL(int serverPort) const {
  return endpoints.auth
    + "?client_id=" + OAuthParameters::ClientId
    + "&redirect_uri=" + OAuthParameters::RedirectUri + ":" + QString::number(serverPort)
    + "&response_type=" + OAuthParameters::ResponseType
    + "&scope=" + OAuthParameters::Scope
    + "&state=" + OAuthParameters::State
    + "&prompt=" + OAuthParameters::Prompt;
}
} // namespace OAuth
