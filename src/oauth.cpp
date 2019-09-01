#include "oauth.h"
#include <QUuid>
#include <QUrl>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>

namespace GS
{
const QString OAuth2::OAuthParameters::State{QCryptographicHash::hash(QUuid::createUuid().toByteArray(QUuid::WithoutBraces), QCryptographicHash::Sha3_256).toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals)};

OAuth2::OAuth2(const QString& clientId, const QString& redirectUri)
  : client_id{clientId.toUtf8()}, redirect_uri{redirectUri.toUtf8()} {
  LogOut();
}

QString OAuth2::GetProfileName() const { return profile.name; }
QString OAuth2::GetProfilePictureURL() const { return profile.pictureURL; }
QString OAuth2::GetAuthEndpoint() const { return endpoints.auth; }
QString OAuth2::GetUserInfoEndpoint() const { return endpoints.userInfo; }
QString OAuth2::GetRedirectUri() const { return redirect_uri; }
QByteArray OAuth2::GetAccessToken() const { return tokens.accessToken; }

void OAuth2::SetAuthEndpoint(const QString& value) { endpoints.auth = value; }
void OAuth2::SetUserInfoEndpoint(const QString& value) { endpoints.userInfo = value; }
void OAuth2::SetUser(const QByteArray& userInfoResponse) {
  QJsonObject userInfo{QJsonDocument::fromJson(userInfoResponse).object()};
  if (userInfo.isEmpty()) return;

  profile.name = userInfo["name"].toString().toUtf8();
  profile.pictureURL = userInfo.contains("picture") ?
                       userInfo["picture"].toString().toUtf8() : QString{""};
}

void OAuth2::LogOut() {
  profile.name = "";
  profile.pictureURL = "";
  tokens.accessToken = "";
}

QString OAuth2::BuildURL(int serverPort) const {
  return GetAuthEndpoint()
    + "?client_id=" + client_id
    + "&redirect_uri=" + redirect_uri + ":" + QString::number(serverPort)
    + "&response_type=" + OAuthParameters::ResponseType
    + "&scope=" + QUrl::toPercentEncoding(OAuthParameters::Scope)
    + "&state=" + OAuthParameters::State
    + "&prompt=" + OAuthParameters::Prompt;
}

OAuthReturnCodes OAuth2::HandleConsent(const QByteArray& consentResponse) {
  QString response{consentResponse};
  int firstIndex{response.lastIndexOf("{")};
  QJsonObject responseAsJson = QJsonDocument::fromJson(
    QStringRef{&response, firstIndex, response.length() - firstIndex}.toUtf8()
  ).object();

  if (responseAsJson.isEmpty()) { return OAuthReturnCodes::CONSENT_ERR; }
  else if (responseAsJson.contains("error"))
  {
    if (responseAsJson["error"] == "access_denied") return OAuthReturnCodes::CONSENT_DENIED;
    return OAuthReturnCodes::CONSENT_ERR;
  }
  else if (responseAsJson["state"] != OAuthParameters::State)
  {
    return OAuthReturnCodes::NON_UNIQUE_REQUEST;
  }

  tokens.accessToken = responseAsJson["access_token"].toString().toUtf8();
  return OAuthReturnCodes::OK;
}

} // namespace GS
