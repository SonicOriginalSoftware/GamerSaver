#include "oauth.h"
#include <QUuid>
#include <QUrl>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>

namespace GS
{
const QString OAuth2::OAuthParameters::State{QCryptographicHash::hash(QUuid::createUuid().toByteArray(QUuid::WithoutBraces), QCryptographicHash::Sha3_256).toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals)};

const QString& OAuth2::GetProfileName() const { return profile.name; }
const QString& OAuth2::GetProfilePictureURL() const { return profile.pictureURL; }

const QString& OAuth2::GetDiscoveryDocEndpoint() const { return endpoints.discoveryDoc; }
const QString& OAuth2::GetAuthEndpoint() const { return endpoints.auth; }
const QString& OAuth2::GetUserInfoEndpoint() const { return endpoints.userInfo; }
void OAuth2::SetAuthEndpoint(const QString& value) { endpoints.auth = value; }
void OAuth2::SetUserInfoEndpoint(const QString& value) { endpoints.userInfo = value; }

const QByteArray& OAuth2::GetAccessToken() const { return tokens.accessToken; }
QString OAuth2::GetRedirectUri() const { return redirect_uri; }

const QString OAuth2::BuildURL(int serverPort) const
{
  return GetAuthEndpoint()
    + "?client_id=" + client_id
    + "&redirect_uri=" + redirect_uri + ":" + QString::number(serverPort)
    + "&response_type=" + OAuthParameters::TokenResponseType
    + "&scope=" + QUrl::toPercentEncoding(OAuthParameters::Scope)
    + "&state=" + OAuthParameters::State
    + "&prompt=" + OAuthParameters::Prompt;
}

OAuth2::OAuth2(const QString& clientId, const QString& redirectUri)
  : client_id{clientId.toUtf8()}, redirect_uri{redirectUri.toUtf8()}
{
  LogOut();
}

ReturnCodes OAuth2::HandleConsent(const QByteArray& consentResponse)
{
  // TODO Handle case where consentResponse = ""

  QString response{consentResponse};
  int firstIndex{response.lastIndexOf("{")};
  QJsonObject responseAsJson = QJsonDocument::fromJson(
    QStringRef{&response, firstIndex, response.length() - firstIndex}.toUtf8()
  ).object();

  if (responseAsJson["error"].toString() != "")
  {
    // TODO Check if consent was denied
      //return ReturnCodes::CONSENT_DENIED;
    return ReturnCodes::CONSENT_ERR;
  }
  else if (responseAsJson["state"] != OAuthParameters::State)
  {
    return ReturnCodes::NON_UNIQUE_REQUEST;
  }

  tokens.accessToken = responseAsJson["access_token"].toString().toUtf8();
  return ReturnCodes::OK;
}

void OAuth2::SetUser(const QByteArray& userInfoResponse)
{
  QJsonObject userInfo{QJsonDocument::fromJson(userInfoResponse).object()};

  profile.name = userInfo["name"].toString().toUtf8();
  profile.pictureURL = userInfo["picture"].toString().toUtf8();
}

void OAuth2::LogOut()
{
  profile.name = "";
  profile.pictureURL = "";
  tokens.accessToken = "";
}
} // namespace GS
