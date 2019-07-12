#include <QFile>
#include <QString>
#include <QUuid>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTcpServer>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include "oauth.h"

const QString GS::OAuth2::Credentials::client_id{"207822922610-jpk6ice7tsbo5ml6ig2q795ph1dnohle.apps.googleusercontent.com"};
const QString GS::OAuth2::Credentials::redirect_uri{"http://localhost"};
const QString GS::OAuth2::OAuthEndpoints::GoogleDiscoveryDoc{"https://accounts.google.com/.well-known/openid-configuration"};
const QString GS::OAuth2::DiscoveryDocKeyNames::AuthorizationEndpointKeyName{"authorization_endpoint"};
const QString GS::OAuth2::DiscoveryDocKeyNames::UserInfoEndpointKeyName{"userinfo_endpoint"};
const QString GS::OAuth2::OAuthParameters::TokenResponseType{"token"};
const QString GS::OAuth2::OAuthParameters::Scope{"https://www.googleapis.com/auth/drive.file profile"};
const QString GS::OAuth2::OAuthParameters::Prompt{"select_account"};
const QString GS::OAuth2::OAuthParameters::State{QCryptographicHash::hash(QUuid::createUuid().toByteArray(QUuid::WithoutBraces), QCryptographicHash::Sha3_256).toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals)};
const QString GS::OAuth2::UserProfile::defaultName{"Login"};
const QString GS::OAuth2::UserProfile::defaultURL{":/res/login_placeholder.svg"};
const QByteArray GS::OAuth2::okResponse{"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"};
const QByteArray GS::OAuth2::responseHTML{"<!DOCTYPE html><html lang='en'><head><title>GamerSaver</title></head><body>You may now close this window <script>const y = window.location.hash.replace('#', '').split('&');const r = {};y.forEach(entry => {const s = entry.split('=');r[s[0]] = s[1]});fetch('/', {method:'POST', headers:{'Content-Type':'application/json'}, body:JSON.stringify(r)})</script></body></html>"};

GS::OAuth2::OAuth2() : profilePictureFileName(QCoreApplication::applicationDirPath() + "profilePic.jpg"),
                       qnam(new QNetworkAccessManager()),
                       dialog(new QMessageBox{QMessageBox::Icon::NoIcon,
                                              "GamerSaver - Waiting...",
                                              "Press Cancel to Abort the request",
                                              QMessageBox::Cancel})
{
  errored = true;
  switch (populateGoogleEndpoints())
  {
    case ReturnCodes::CANCELLED:
      qDebug() << "Discovery request cancelled!";
      return;
    case ReturnCodes::SSL_ERR:
      qDebug() << "SSL Errors were generated! Make sure you have an SSL implementation installed!";
      return;
    case ReturnCodes::NETWORK_ERR:
      qDebug() << "Discovery endpoint could not be reached!";
      return;
  }
  errored = false;
}

GS::OAuth2::~OAuth2() { delete qnam; delete dialog; }

bool GS::OAuth2::LoggedIn() const { return loggedIn; }
bool GS::OAuth2::Errored() const { return errored; }
const QString& GS::OAuth2::ProfileName() const { return profile.name; }
const QString& GS::OAuth2::ProfilePictureURL() { return loggedIn ? profilePictureFileName : UserProfile::defaultURL; }
const QString& GS::OAuth2::DefaultPictureURL() { return UserProfile::defaultURL; }
const QString& GS::OAuth2::DefaultName() { return UserProfile::defaultName; }

void GS::OAuth2::shutdownServer(QTcpServer& loopbackServer) const
{
  loopbackServer.close();
  qDebug() << "Server stopped listening";
}

GS::OAuth2::ReturnCodes GS::OAuth2::awaitAndRespondOnLoopback(const QByteArray& response, QByteArray& reply) const
{
  dialog->exec();

  QTcpSocket *connection{loopbackServer.nextPendingConnection()};
  if (dialog->result() == QMessageBox::StandardButton::Cancel || connection == nullptr) return ReturnCodes::CANCELLED;

  connection->write(response);
  connection->flush();
  // Will this just wait forever if there's no writing coming back from the server?
  connection->waitForReadyRead();
  reply = connection->readAll();
  connection->disconnectFromHost();
  delete connection; // BECAUSE QT PASSES BACK A FREE MEMORY OBJECT!?!?
  return ReturnCodes::OK;
}

// FIXME Look how much repeated code there is in here!!??
GS::OAuth2::ReturnCodes GS::OAuth2::promptForConsent(QByteArray& consentResponse) const
{
  QTcpServer loopbackServer{};
  if (!loopbackServer.listen(QHostAddress(Credentials::redirect_uri))) return;

  connect(&loopbackServer, &QTcpServer::newConnection, dialog, &QMessageBox::accept);
  if (!QDesktopServices::openUrl(QUrl{
    endpoints.GoogleAuthServer
    + "?client_id=" + Credentials::client_id
    + "&redirect_uri=" + Credentials::redirect_uri + ":" + QString::number(loopbackServer.serverPort())
    + "&response_type=" + OAuthParameters::TokenResponseType
    + "&scope=" + QUrl::toPercentEncoding(OAuthParameters::Scope)
    + "&state=" + OAuthParameters::State
    + "&prompt=" + OAuthParameters::Prompt
  }))
  {
    shutdownServer(loopbackServer);
    return ReturnCodes::UNHANDLED;
  }

  if (awaitAndRespondOnLoopback(QByteArray{okResponse + responseHTML}, QByteArray{""}) != ReturnCodes::OK)
  {
    shutdownServer(loopbackServer);
    return ReturnCodes::CANCELLED;
  }
  connect(&loopbackServer, &QTcpServer::newConnection, dialog, &QMessageBox::accept);
  if (awaitAndRespondOnLoopback(QByteArray{okResponse}, consentResponse) != ReturnCodes::OK)
  {
    shutdownServer(loopbackServer);
    return ReturnCodes::CANCELLED;
  }

  shutdownServer(loopbackServer);
  return ReturnCodes::OK;
}

GS::OAuth2::ReturnCodes GS::OAuth2::populateGoogleEndpoints()
{
  QByteArray discoveryDoc{""};
  ReturnCodes returnCode{get(QNetworkRequest{QUrl{OAuth2::OAuthEndpoints::GoogleDiscoveryDoc}}, discoveryDoc)};
  if (returnCode != ReturnCodes::OK) return returnCode;

  QJsonObject discoveryDocObject{QJsonDocument::fromJson(discoveryDoc).object()};
  endpoints.GoogleAuthServer = discoveryDocObject[DiscoveryDocKeyNames::AuthorizationEndpointKeyName].toString();
  endpoints.GoogleUserInfo = discoveryDocObject[DiscoveryDocKeyNames::UserInfoEndpointKeyName].toString();
  return OK;
}

GS::OAuth2::ReturnCodes GS::OAuth2::get(const QNetworkRequest& request, QByteArray& response) const
{
  connect(qnam, &QNetworkAccessManager::finished, dialog, &QMessageBox::accept);
  QNetworkReply *reply{qnam->get(request)};
  dialog->exec();
  reply->deleteLater();

  if (dialog->result() == QMessageBox::StandardButton::Cancel) return ReturnCodes::CANCELLED;

  switch (reply->error())
  {
    case QNetworkReply::SslHandshakeFailedError:
      return ReturnCodes::SSL_ERR;
    default:
      return ReturnCodes::NETWORK_ERR;
  }
  response = reply->readAll();
  return ReturnCodes::OK;
}

void GS::OAuth2::LogOut()
{
  QFile::remove(profilePictureFileName);

  loggedIn = false;
  errored = false;
  profile.name = UserProfile::defaultName;
  profile.pictureURL = UserProfile::defaultURL;
  tokens.accessToken = "";
}

void GS::OAuth2::LogIn()
{
  // FIXME CLEAN ALL THIS UP
  // TODO Clean up network reqests
  //
  // TODO Clean up the server response/write/response
  //
  // TODO MAKE SURE THE NAME AND PICTURE URL WILL RESET BACK TO DEFAULTS
  // EVEN IF THE USER CANCELS A NETWORK REQUEST
  LogOut();
  errored = true;

  QByteArray consentResponse{};
  switch (promptForConsent(consentResponse))
  {
    case (ReturnCodes::UNHANDLED):
      qDebug() << "Could not handle opening URL in default system browser!";
      return;
    case (ReturnCodes::CANCELLED):
      qDebug() << "Request for consent was cancelled!";
      return;
  }

  QString response{consentResponse};
  int firstIndex{response.lastIndexOf("{")};
  QJsonObject json{QJsonDocument::fromJson(QStringRef{&response, firstIndex, response.length() - firstIndex}.toUtf8()).object()};

  if (json["error"].toString() != "") { qDebug() << "Error:" << json["error"]; return; }
  else if (json["state"] != OAuthParameters::State) { qDebug() << "Request not unique!"; return; }

  tokens.accessToken = json["access_token"].toString();

  QNetworkRequest userInfoRequest{QUrl{endpoints.GoogleUserInfo}};
  userInfoRequest.setRawHeader(QByteArray{"Authorization"}, QByteArray{"Bearer " + tokens.accessToken.toUtf8()});
  QByteArray userInfoResponse{};
  switch (get(userInfoRequest, userInfoResponse))
  {
    case ReturnCodes::OK:
      break;
    case ReturnCodes::CANCELLED:
      qDebug() << "User info request cancelled!";
      return;
    default:
      qDebug() << "Could not retrieve user info!";
      return;
  }

  QJsonObject userInfo{QJsonDocument::fromJson(userInfoResponse).object()};

  profile.name = userInfo["name"].toString();
  profile.pictureURL = userInfo["picture"].toString();
  loggedIn = true;

  // TODO CLEAN UP THIS PICTURE REQUESTS AND WRITES
  // AND THINK IF THE USER SHOULD REALLY BE 'LOGGED IN' AT THIS POINT
  QNetworkReply* pictureReply;
  connect(qnam, &QNetworkAccessManager::finished, dialog, &QMessageBox::accept);
  pictureReply = qnam->get(QNetworkRequest{QUrl{profile.pictureURL}});
  dialog->exec();

  pictureReply->deleteLater();
  if (dialog->result() == QMessageBox::StandardButton::Cancel || pictureReply->error() != QNetworkReply::NoError)
  {
    qDebug() << "Could not get profile picture!";
    return;
  }

  QFile profilePictureFile{profilePictureFileName};
  if (!profilePictureFile.open(QFile::WriteOnly))
  {
    qDebug() << "Could not open profile picture file!";
    return;
  }

  QByteArray profilePicture{pictureReply->readAll()};
  qDebug() << "Writing profile picture to" << profilePictureFileName;
  if (profilePictureFile.write(profilePicture) != profilePicture.size()) qDebug() << "Could not write the profile picture!";

  errored = false;
  return;
}
