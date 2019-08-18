#include <QObject>
#include <QFile>
#include <QString>
#include <QUuid>
#include <QCoreApplication>
#include <QtWidgets/QStatusBar>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTcpServer>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include "oauth.h"

namespace GS
{
const QString OAuth2::Credentials::client_id{"207822922610-jpk6ice7tsbo5ml6ig2q795ph1dnohle.apps.googleusercontent.com"};
const QString OAuth2::Credentials::redirect_uri{"http://localhost"};
const QString OAuth2::OAuthEndpoints::GoogleDiscoveryDoc{"https://accounts.google.com/.well-known/openid-configuration"};
const QString OAuth2::DiscoveryDocKeyNames::AuthorizationEndpointKeyName{"authorization_endpoint"};
const QString OAuth2::DiscoveryDocKeyNames::UserInfoEndpointKeyName{"userinfo_endpoint"};
const QString OAuth2::OAuthParameters::TokenResponseType{"token"};
const QString OAuth2::OAuthParameters::Scope{"https://www.googleapis.com/auth/drive.file profile"};
const QString OAuth2::OAuthParameters::Prompt{"select_account"};
const QString OAuth2::OAuthParameters::State{QCryptographicHash::hash(QUuid::createUuid().toByteArray(QUuid::WithoutBraces), QCryptographicHash::Sha3_256).toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals)};
const QString OAuth2::UserProfile::defaultName{"Login"};
const QString OAuth2::UserProfile::defaultURL{":/res/login_placeholder.svg"};
const QByteArray OAuth2::okResponse{"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"};
const QByteArray OAuth2::responseHTML{"<!DOCTYPE html><html lang='en'><head><title>GamerSaver</title></head><body>You may now close this window <script>const y = window.location.hash.replace('#', '').split('&');const r = {};y.forEach(entry => {const s = entry.split('=');r[s[0]] = s[1]});fetch('/', {method:'POST', headers:{'Content-Type':'application/json'}, body:JSON.stringify(r)})</script></body></html>"};

const bool OAuth2::SSLSupported() const { return supportsSSL; }
const QString& OAuth2::ProfileName() const { return profile.name; }
const QString& OAuth2::ProfilePictureURL() { return QFile::exists(profilePictureFileName) ? profilePictureFileName : UserProfile::defaultURL; }
const QString& OAuth2::DefaultPictureURL() { return UserProfile::defaultURL; }
const QString& OAuth2::DefaultName() { return UserProfile::defaultName; }

OAuth2::OAuth2(QNetworkAccessManager& qnam, QTcpServer& server) :
  profilePictureFileName{QCoreApplication::applicationDirPath() + "profilePic.jpg"},
  qnam{qnam},
  server{server}
{
  LogOut();
  if (!QSslSocket::supportsSsl()) supportsSSL = false;
}

const ReturnCodes OAuth2::get(const QNetworkRequest& request, QByteArray& response) const
{
  ReturnCodes returnCode{ReturnCodes::CANCELLED};
  QNetworkReply* reply{qnam.get(request)};
  if (dialog->exec() == QDialog::Accepted)
  {
    returnCode = (reply->error() == QNetworkReply::NoError) ? ReturnCodes::OK : ReturnCodes::NETWORK_ERR;
    response = reply->readAll();
  }

  delete reply;
  return returnCode;
}

const ReturnCodes OAuth2::awaitAndRespondToServer()
{
  if (!server.listen(QHostAddress(Credentials::redirect_uri))) return ReturnCodes::SERVER_ERR;
  dialog->exec();

  QTcpSocket *connection{server.nextPendingConnection()};
  if (dialog->result() == QMessageBox::StandardButton::Cancel || connection == nullptr) return ReturnCodes::CANCELLED;

  connection->write(okResponse + responseHTML);
  connection->flush();
  connection->waitForReadyRead();
  consentResponse = connection->readAll();
  connection->disconnectFromHost();
  delete connection;
  server.close();
  return ReturnCodes::OK;
}

const ReturnCodes OAuth2::PopulateGoogleEndpoints()
{
  QByteArray discoveryDoc{""};
  ReturnCodes returnCode{get(QNetworkRequest{QUrl{OAuth2::OAuthEndpoints::GoogleDiscoveryDoc}}, discoveryDoc)};
  if (returnCode != ReturnCodes::OK) return returnCode;

  QJsonObject discoveryDocObject{QJsonDocument::fromJson(discoveryDoc).object()};
  endpoints.GoogleAuthServer = discoveryDocObject[DiscoveryDocKeyNames::AuthorizationEndpointKeyName].toString();
  endpoints.GoogleUserInfo = discoveryDocObject[DiscoveryDocKeyNames::UserInfoEndpointKeyName].toString();
  return OK;
}

const ReturnCodes OAuth2::PromptForConsent()
{
  ReturnCodes returnCode{ReturnCodes::UNHANDLED};
  if (QDesktopServices::openUrl(
    QUrl{endpoints.GoogleAuthServer
    + "?client_id=" + Credentials::client_id
    + "&redirect_uri=" + Credentials::redirect_uri + ":" + QString::number(server.serverPort())
    + "&response_type=" + OAuthParameters::TokenResponseType
    + "&scope=" + QUrl::toPercentEncoding(OAuthParameters::Scope)
    + "&state=" + OAuthParameters::State
    + "&prompt=" + OAuthParameters::Prompt}))
  {
    returnCode = ReturnCodes::CANCELLED;
    if (awaitAndRespondToServer() == ReturnCodes::OK)
    {
      if (awaitAndRespondToServer() == ReturnCodes::OK)
      {
        returnCode = ReturnCodes::OK;
      }
    }
  }
  return returnCode;
}

const ReturnCodes OAuth2::HandleConsent()
{
  QString response{*consentResponse};
  int firstIndex{response.lastIndexOf("{")};
  responseAsJson = QJsonDocument::fromJson(
    QStringRef{&response, firstIndex, response.length() - firstIndex}.toUtf8()
  ).object();

  if (responseAsJson["error"].toString() != "")
  {
    // TODO Check if consent was denied
    return ReturnCodes::CONSENT_ERR;
  }
  else if (responseAsJson["state"] != OAuthParameters::State)
  {
    return ReturnCodes::NON_UNIQUE_REQUEST;
  }

  tokens.accessToken = responseAsJson["access_token"].toString().toUtf8();
  return ReturnCodes::OK;
}

const ReturnCodes OAuth2::SetUser()
{
  QNetworkRequest userInfoRequest{QUrl{endpoints.GoogleUserInfo}};
  userInfoRequest.setRawHeader("Authorization", "Bearer " + tokens.accessToken);
  QByteArray userInfoResponse{""};
  switch (get(userInfoRequest, userInfoResponse))
  {
    case ReturnCodes::OK:
      break;
    case ReturnCodes::CANCELLED:
      return ReturnCodes::CANCELLED;
    default:
      return ReturnCodes::NETWORK_ERR;
  }

  QJsonObject userInfo{QJsonDocument::fromJson(userInfoResponse).object()};

  profile.name = userInfo["name"].toString();
  profile.pictureURL = userInfo["picture"].toString();
  return ReturnCodes::OK;
}

const ReturnCodes OAuth2::UpdateProfilePicture() const
{
  QByteArray profilePicture{""};
  if (get(QNetworkRequest{QUrl{profile.pictureURL}}, profilePicture) != ReturnCodes::OK) return ReturnCodes::NETWORK_ERR;

  QFile profilePictureFile{profilePictureFileName};
  if (!profilePictureFile.open(QFile::WriteOnly)) return ReturnCodes::IO_ERR;

  profilePictureFile.write(profilePicture);
  return ReturnCodes::OK;
}

const void OAuth2::LogOut()
{
  QFile::remove(profilePictureFileName);

  profile.name = UserProfile::defaultName;
  profile.pictureURL = UserProfile::defaultURL;
  tokens.accessToken = "";
}
}
