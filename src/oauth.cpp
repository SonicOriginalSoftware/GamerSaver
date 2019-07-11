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

GS::OAuth2::OAuth2() : profilePictureFileName(QCoreApplication::applicationDirPath() + "profilePic.jpg"),
                       qnam(new QNetworkAccessManager()),
                       dialog(new QMessageBox{QMessageBox::Icon::NoIcon,
                                              "GamerSaver - Waiting...",
                                              "Press Cancel to Abort the request",
                                              QMessageBox::Cancel})
{}

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

void GS::OAuth2::shutdownServer(QTcpServer& loopbackServer, QTcpSocket* connection) const
{
  if (connection != 0) { connection->disconnectFromHost(); connection->deleteLater(); }
  shutdownServer(loopbackServer);
}

void GS::OAuth2::LogOut()
{
  profile.name = UserProfile::defaultName;
  profile.pictureURL = UserProfile::defaultURL;
  QFile::remove(profilePictureFileName);
  tokens.accessToken = "";
  loggedIn = false;
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

  QFile responseHTMLFile{":/res/response.html"};
  if (!responseHTMLFile.open(QFile::OpenModeFlag::ReadOnly)) return;
  QByteArray responseHTML{responseHTMLFile.readAll()};
  responseHTMLFile.close();

  QTcpServer loopbackServer{};
  if (!loopbackServer.listen(QHostAddress(Credentials::redirect_uri))) return;

  QNetworkReply* discoveryReply;
  connect(qnam, &QNetworkAccessManager::finished, dialog, &QMessageBox::accept);
  discoveryReply = qnam->get(QNetworkRequest{QUrl{OAuthEndpoints::GoogleDiscoveryDoc}});
  dialog->exec();

  discoveryReply->deleteLater();
  if (dialog->result() == QMessageBox::StandardButton::Cancel || discoveryReply->error() != QNetworkReply::NoError)
  {
    qDebug() << "Did not get discovery file!";
    errored = true;
    return;
  }

  QJsonObject discoveryDoc{QJsonDocument::fromJson(discoveryReply->readAll()).object()};
  endpoints.GoogleAuthServer = discoveryDoc[DiscoveryDocKeyNames::AuthorizationEndpointKeyName].toString();
  endpoints.GoogleUserInfo = discoveryDoc[DiscoveryDocKeyNames::UserInfoEndpointKeyName].toString();

  QString requestString{
    endpoints.GoogleAuthServer
    + "?client_id=" + Credentials::client_id
    + "&redirect_uri=" + Credentials::redirect_uri + ":" + QString::number(loopbackServer.serverPort())
    + "&response_type=" + OAuthParameters::TokenResponseType
    + "&scope=" + QUrl::toPercentEncoding(OAuthParameters::Scope)
    + "&state=" + OAuthParameters::State
    + "&prompt=" + OAuthParameters::Prompt
  };

  connect(&loopbackServer, &QTcpServer::newConnection, dialog, &QMessageBox::accept);

  qDebug() << "Navigating to" << requestString << "for OAuth";
  QDesktopServices::openUrl(QUrl{requestString});
  dialog->exec();

  QTcpSocket *connection{loopbackServer.nextPendingConnection()};
  if (dialog->result() == QMessageBox::StandardButton::Cancel || connection == nullptr)
  {
    qDebug() << "Request was cancelled";
    shutdownServer(loopbackServer);
    return;
  }

  connection->write(okResponse.constData(), okResponse.size());
  connection->write(responseHTML.constData(), responseHTML.size());
  connection->flush();

  // Wait for the page to send back the access_token or error information
  connect(&loopbackServer, &QTcpServer::newConnection, dialog, &QMessageBox::accept);
  dialog->exec();

  connection = loopbackServer.nextPendingConnection();
  if (dialog->result() == QMessageBox::StandardButton::Cancel || connection == nullptr)
  {
    qDebug() << "Request was cancelled";
    shutdownServer(loopbackServer);
    return;
  }

  connection->write(okResponse.constData(), okResponse.size());
  connection->flush();

  QByteArray responseData;
  connection->waitForReadyRead();
  responseData.append(connection->readAll());

  shutdownServer(loopbackServer, connection);

  QString response{responseData};
  int firstIndex{response.lastIndexOf("{")};
  QJsonObject json{QJsonDocument::fromJson(QStringRef{&response, firstIndex, response.length() - firstIndex}.toUtf8()).object()};

  if (json["error"].toString() != "") { qDebug() << "Error:" << json["error"]; return; }
  else if (json["state"] != OAuthParameters::State) { qDebug() << "Request not unique!"; return; }

  tokens.accessToken = json["access_token"].toString();

  QNetworkReply *userInfoReply;
  connect(qnam, &QNetworkAccessManager::finished, dialog, &QMessageBox::accept);
  QNetworkRequest userInfoRequest{QUrl{endpoints.GoogleUserInfo}};
  userInfoRequest.setRawHeader(QByteArray{"Authorization"}, QByteArray{"Bearer " + tokens.accessToken.toUtf8()});
  userInfoReply = qnam->get(userInfoRequest);
  dialog->exec();

  userInfoReply->deleteLater();
  if (dialog->result() == QMessageBox::StandardButton::Cancel || userInfoReply->error() != QNetworkReply::NoError)
  {
    qDebug() << "Could not retrieve user info!";
    errored = true;
    return;
  }
  QByteArray userInfoResponse{userInfoReply->readAll()};
  QJsonObject userInfo{QJsonDocument::fromJson(userInfoResponse).object()};

  profile.name = userInfo["name"].toString();
  profile.pictureURL = userInfo["picture"].toString();
  loggedIn = true;

  QNetworkReply* pictureReply;
  connect(qnam, &QNetworkAccessManager::finished, dialog, &QMessageBox::accept);
  pictureReply = qnam->get(QNetworkRequest{QUrl{profile.pictureURL}});
  dialog->exec();

  pictureReply->deleteLater();
  if (dialog->result() == QMessageBox::StandardButton::Cancel || pictureReply->error() != QNetworkReply::NoError)
  {
    qDebug() << "Could not get profile picture!";
    errored = true;
    return;
  }

  QFile profilePictureFile{profilePictureFileName};
  if (!profilePictureFile.open(QFile::WriteOnly))
  {
    qDebug() << "Could not open profile picture file!";
    errored = true;
    return;
  }

  QByteArray profilePicture{pictureReply->readAll()};
  qDebug() << "Writing profile picture to" << profilePictureFileName;
  qint64 bytesWritten{profilePictureFile.write(profilePicture)};
  if (bytesWritten == -1 || bytesWritten != profilePicture.size())
  {
    qDebug() << "Could not write the profile picture!";
    errored = true;
  }
  profilePictureFile.close();

  return;
}

