#include "oauth.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QUuid>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QTcpServer>
#include <QMessageBox>
#include <QEventLoop>

struct GS::OAuth2::Credentials
{
  QString client_id;
  QString auth_uri;
  QString token_uri;
  QStringList redirect_uris;
};

struct GS::OAuth2::Tokens
{
  QString accessToken;
  QString refreshToken;
};

struct GS::OAuth2::OAuthParameters
{
  static const QString AuthCodeResponseType;
  static const QString Scope;
  static const QString CodeChallengeMethod;
};

struct GS::OAuth2::OAuthEndpoints
{
  static const QString GoogleAuthServer;
};

struct GS::OAuth2::UserProfile
{
  QString profileURL;
};

struct GS::OAuth2::OAuthResponse
{
  static const QByteArray okResponse;
};

const QString GS::OAuth2::OAuthEndpoints::GoogleAuthServer{"https://accounts.google.com/o/oauth2/v2/auth"};
const QString GS::OAuth2::OAuthParameters::AuthCodeResponseType{"code"};
const QString GS::OAuth2::OAuthParameters::Scope{"https://www.googleapis.com/auth/drive.file"};
const QString GS::OAuth2::OAuthParameters::CodeChallengeMethod{"S256"};
const QByteArray GS::OAuth2::OAuthResponse::okResponse{
  "HTTP/1.1 200 OK\r\n"
  "Connection: close\r\n"
  "Content-Type: text/html; charset=UTF-8\r\n\r\n"
};

GS::OAuth2::OAuth2() : credentials(new Credentials()),
                       tokens(new Tokens()),
                       profile(new UserProfile())
{
  QFile credentialFile{":/res/apikey.json"};
  if (!credentialFile.open(QFile::ReadOnly)) {
    errored = true;
    return;
  }
  QByteArray jsonData{credentialFile.readAll()};
  credentialFile.close();

  QJsonParseError jsonErr{};
  QJsonDocument credentialJson{QJsonDocument::fromJson(jsonData, &jsonErr)};
  if (jsonErr.error != QJsonParseError::NoError) {
    errored = true;
    return;
  }

  QJsonObject json{credentialJson.object()};
  json = json["installed"].toObject();

  credentials->client_id = json["client_id"].toString();
  credentials->auth_uri = json["auth_uri"].toString();
  credentials->token_uri = json["token_uri"].toString();
  credentials->redirect_uris = json["redirect_uris"].toVariant().toStringList();
}

GS::OAuth2::~OAuth2()
{
  delete credentials;
  delete tokens;
  delete profile;
}

void GS::OAuth2::shutdownServer(QTcpServer& loopbackServer) const
{
  loopbackServer.close();
  qDebug() << "Server stopped listening";
}

void GS::OAuth2::shutdownServer(QTcpServer& loopbackServer, QTcpSocket* connection) const
{
  if (connection != 0)
  {
    connection->disconnectFromHost();
    connection->deleteLater();
  }
  shutdownServer(loopbackServer);
}

bool GS::OAuth2::Errored() const { return errored; }

void GS::OAuth2::RequestLogin(const QNetworkAccessManager& qnam) const
{
  // TODO Check for existing access token and refresh token
  // If access token is found, check that it is valid
  // If it is no longer valid, renew it using the refresh token
  // If the refresh token is no longer valid, proceed with below

  QFile responseHTMLFile{":/res/response.html"};
  QByteArray responseHTML;
  if (responseHTMLFile.open(QFile::OpenModeFlag::ReadOnly))
  {
    responseHTML = responseHTMLFile.readAll();
    responseHTMLFile.close();
  } else { return; }

  QTcpServer loopbackServer{};
  if (!loopbackServer.listen(QHostAddress(credentials->redirect_uris[1]))) return;
  qDebug() << "Listening on: " << loopbackServer.serverAddress() << ":" << loopbackServer.serverPort();

  QByteArray code_verifier{QUuid::createUuid().toByteArray(QUuid::WithoutBraces)};
  QByteArray code_challenge{QCryptographicHash::hash(code_verifier, QCryptographicHash::Sha3_256)};
  code_challenge = code_challenge.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);

  QString state{QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces)};

  QString requestString{
    OAuthEndpoints::GoogleAuthServer
    + "?client_id=" + credentials->client_id
    + "&redirect_uri=" + credentials->redirect_uris[1] + ":" + QString::number(loopbackServer.serverPort())
    + "&response_type=" + GS::OAuth2::OAuthParameters::AuthCodeResponseType
    + "&scope=" + GS::OAuth2::OAuthParameters::Scope
    + "&code_challenge_method=" + GS::OAuth2::OAuthParameters::CodeChallengeMethod
    + "&code_challenge=" + QUrl::toPercentEncoding(QString(code_challenge))
    + "&state=" + state
  };

  QMessageBox dialog;
  dialog.setWindowTitle("GamerSaver - Waiting...");
  dialog.setText("Press Cancel to Abort the request");
  dialog.setStandardButtons(QMessageBox::Cancel);
  connect(&loopbackServer, &QTcpServer::newConnection, &dialog, &QMessageBox::accept);
  QDesktopServices::openUrl(QUrl{requestString});
  dialog.exec();

  QTcpSocket *connection{loopbackServer.nextPendingConnection()};
  if (dialog.result() == QMessageBox::StandardButton::Cancel || connection == 0)
  {
    qDebug() << "Request was cancelled";
    shutdownServer(loopbackServer);
    return;
  }

  connection->write(OAuthResponse::okResponse.constData(), OAuthResponse::okResponse.size());
  connection->write(responseHTML);

  QByteArray responseData;
  connection->waitForReadyRead();
  responseData.append(connection->readAll());

  shutdownServer(loopbackServer, connection);

  // TODO Check if the user granted permission
  // If they did...
  // TODO Parse the returned profile image URL
  QString imageURL{};
  profile->profileURL = imageURL;

  // TODO Parse the returned authorization code
  QString authCode{};

  // TODO Use the authcode to send a new request to get the refresh and access tokens

  // TODO Set the access and refresh tokens
  QString returnedAccessToken{};
  tokens->accessToken = returnedAccessToken;
  QString returnedRefreshToken{};
  tokens->refreshToken = returnedRefreshToken;

  // TODO Store the access and refresh tokens

  return;
}

const QString& GS::OAuth2::ProfileImageURL() const
{
  return profile->profileURL;
}

