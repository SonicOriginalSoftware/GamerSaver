#include "googleoauth.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QtNetwork/QTcpServer>

namespace GS {
const QString OAuth2::OAuthEndpoints::discoveryDoc{ "https://accounts.google.com/.well-known/openid-configuration"};
const QString OAuth2::OAuthParameters::TokenResponseType{"token"};
const QString OAuth2::OAuthParameters::Scope{ "https://www.googleapis.com/auth/drive.file profile"};
const QString OAuth2::OAuthParameters::Prompt{"select_account"};

const QString GoogleOAuth::authorizationEndpointKeyName{ "authorization_endpoint"};
const QString GoogleOAuth::userInfoEndpointKeyName{"userinfo_endpoint"};

QString GoogleOAuth::GetAuthEndpointKeyName() const { return authorizationEndpointKeyName; }
QString GoogleOAuth::GetUserInfoEndpointKeyName() const { return userInfoEndpointKeyName; }
} // namespace GS

