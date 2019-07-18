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

GS::OAuth2::OAuth2(QStatusBar* mainWindowStatusBar) :
  profilePictureFileName{QCoreApplication::applicationDirPath() + "profilePic.jpg"},
  statusBar{mainWindowStatusBar},
  qnam{new QNetworkAccessManager()},
  dialog{new QMessageBox{QMessageBox::Icon::NoIcon,
                         "GamerSaver - Waiting...",
                         "Press Cancel to abort the request",
                         QMessageBox::Cancel}}
{
  LogOut();

  QObject::connect(qnam, &QNetworkAccessManager::finished, dialog, &QMessageBox::accept);
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
  statusBar->showMessage("Server stopped listening", messageTimeout);
}

GS::OAuth2::ReturnCodes GS::OAuth2::awaitAndRespondOnLoopback(QTcpServer& loopbackServer, const QByteArray& response) const
{
  QByteArray dummy{""};
  return awaitAndRespondOnLoopback(loopbackServer, response, dummy);
}

void GS::OAuth2::LogOut()
{
  QFile::remove(profilePictureFileName);

  loggedIn = false;
  profile.name = UserProfile::defaultName;
  profile.pictureURL = UserProfile::defaultURL;
  tokens.accessToken = "";
  statusBar->showMessage("Logged out");
  errored = false;
}

GS::OAuth2::ReturnCodes GS::OAuth2::get(const QNetworkRequest& request, QByteArray& response) const
{
  ReturnCodes returnCode{ReturnCodes::CANCELLED};
  QNetworkReply* reply{qnam->get(request)};
  if (dialog->exec() == QDialog::Accepted)
  {
    returnCode = (reply->error() == QNetworkReply::NoError) ? ReturnCodes::OK : ReturnCodes::NETWORK_ERR;
    response = reply->readAll();
  }

  delete reply;
  return returnCode;
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

QUrl GS::OAuth2::buildPromptURL(const int& port) const
{
  return QUrl{endpoints.GoogleAuthServer
    + "?client_id=" + Credentials::client_id
    + "&redirect_uri=" + Credentials::redirect_uri + ":" + QString::number(port)
    + "&response_type=" + OAuthParameters::TokenResponseType
    + "&scope=" + QUrl::toPercentEncoding(OAuthParameters::Scope)
    + "&state=" + OAuthParameters::State
    + "&prompt=" + OAuthParameters::Prompt};
}

GS::OAuth2::ReturnCodes GS::OAuth2::awaitAndRespondOnLoopback(QTcpServer& loopbackServer, const QByteArray& response, QByteArray& reply) const
{
  dialog->exec();

  QTcpSocket *connection{loopbackServer.nextPendingConnection()};
  if (dialog->result() == QMessageBox::StandardButton::Cancel || connection == nullptr) return ReturnCodes::CANCELLED;

  connection->write(response);
  connection->flush();
  connection->waitForReadyRead();
  reply = connection->readAll();
  connection->disconnectFromHost();
  delete connection; // BECAUSE QT PASSES BACK AN ALLOCATED MEMORY OBJECT!?!?
  return ReturnCodes::OK;
}

GS::OAuth2::ReturnCodes GS::OAuth2::promptForConsent(QByteArray& consentResponse) const
{
  QTcpServer loopbackServer{};
  if (!loopbackServer.listen(QHostAddress(Credentials::redirect_uri))) return ReturnCodes::SERVER_ERR;

  ReturnCodes returnCode{ReturnCodes::UNHANDLED};
  QObject::connect(&loopbackServer, &QTcpServer::newConnection, dialog, &QMessageBox::accept);
  if (QDesktopServices::openUrl(buildPromptURL(loopbackServer.serverPort())))
  {
    returnCode = ReturnCodes::CANCELLED;
    if (awaitAndRespondOnLoopback(loopbackServer, QByteArray{okResponse + responseHTML}, consentResponse) == ReturnCodes::OK)
    {
      if (awaitAndRespondOnLoopback(loopbackServer, QByteArray{okResponse}, consentResponse) == ReturnCodes::OK)
      {
        returnCode = ReturnCodes::OK;
      }
    }
  }

  shutdownServer(loopbackServer);
  return returnCode;
}

void GS::OAuth2::LogIn()
{
  LogOut();
  errored = true;
  statusBar->showMessage("Requesting Google Discovery Doc from " + OAuth2::OAuthEndpoints::GoogleDiscoveryDoc);
  switch (populateGoogleEndpoints())
  {
    case ReturnCodes::CANCELLED:
      statusBar->showMessage("Discovery request cancelled!");
      return;
    case ReturnCodes::NETWORK_ERR:
      statusBar->showMessage("Discovery endpoint could not be reached!");
      return;
    default:
      break;
  }
  statusBar->showMessage("Awaiting user consent...");

  QByteArray consentResponse{};
  switch (promptForConsent(consentResponse))
  {
    case (ReturnCodes::UNHANDLED):
      statusBar->showMessage("Could not handle opening URL in default system browser!");
      return;
    case (ReturnCodes::CANCELLED):
      statusBar->showMessage("Request for consent was cancelled!");
      return;
    case ReturnCodes::SERVER_ERR:
      statusBar->showMessage("The localhost server could not be started!");
    default:
      break;
  }

  QString response{consentResponse};
  int firstIndex{response.lastIndexOf("{")};
  QJsonObject json{QJsonDocument::fromJson(QStringRef{&response, firstIndex, response.length() - firstIndex}.toUtf8()).object()};

  if (json["error"].toString() != "")
  {
    statusBar->showMessage("Error: " + json["error"].toString());
    return;
  }
  else if (json["state"] != OAuthParameters::State)
  {
    statusBar->showMessage("Request was not unique!");
    return;
  }

  tokens.accessToken = json["access_token"].toString();

  QNetworkRequest userInfoRequest{QUrl{endpoints.GoogleUserInfo}};
  userInfoRequest.setRawHeader(QByteArray{"Authorization"}, QByteArray{"Bearer " + tokens.accessToken.toUtf8()});
  QByteArray userInfoResponse{""};
  switch (get(userInfoRequest, userInfoResponse))
  {
    case ReturnCodes::OK:
      break;
    case ReturnCodes::CANCELLED:
      statusBar->showMessage("User info request cancelled!");
      return;
    default:
      statusBar->showMessage("Could not retrieve user info!");
      return;
  }

  QJsonObject userInfo{QJsonDocument::fromJson(userInfoResponse).object()};

  profile.name = userInfo["name"].toString();
  profile.pictureURL = userInfo["picture"].toString();
  loggedIn = true;
  errored = false;
  statusBar->showMessage("Logged in");

  QByteArray profilePicture{""};
  QFile profilePictureFile{profilePictureFileName};
  switch (get(QNetworkRequest{QUrl{profile.pictureURL}}, profilePicture))
  {
    case ReturnCodes::OK:
      if (!profilePictureFile.open(QFile::WriteOnly))
      {
        statusBar->showMessage("Could not open profile picture file!");
        return;
      }
      break;
    case ReturnCodes::CANCELLED:
      statusBar->showMessage("Profile picture request cancelled!");
      return;
    default:
      statusBar->showMessage("Could not retrieve profile picture!");
      return;
  }
  profilePictureFile.write(profilePicture);

  return;
}
