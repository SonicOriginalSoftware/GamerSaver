#include "oauthnetaccess.h"
#include <QByteArray>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslSocket>
#include <QString>

namespace GS {
OAuthNetAccess::OAuthNetAccess(QEventLoop& loop) : _loop{loop}
{
  QObject::connect(&qnam, &QNetworkAccessManager::finished, &_loop, &QEventLoop::quit);
}

bool OAuthNetAccess::SSLSupported() {return QSslSocket::supportsSsl();}

QString OAuthNetAccess::GetSSLBuildVersion() {return QSslSocket::sslLibraryBuildVersionString();}

bool OAuthNetAccess::NetworkConnected() const {return qnam.networkAccessible() == QNetworkAccessManager::NetworkAccessibility::Accessible;}

QByteArray OAuthNetAccess::get(const QNetworkRequest &request)
{
  QByteArray response{""};
  QNetworkReply *reply{qnam.get(request)};
  if (_loop.exec() == 0 && reply->error() == QNetworkReply::NoError) response = reply->readAll();

  delete reply;
  return response;
}

QByteArray OAuthNetAccess::Get(const QString &requestUrl)
{
  return get(QNetworkRequest(QUrl(requestUrl)));
}

QByteArray OAuthNetAccess::Get(const QString &requestUrl, const QByteArray &token)
{
  QNetworkRequest oauthRequest{QUrl{requestUrl}};
  oauthRequest.setRawHeader("Authorization", "Bearer " + token);
  return get(oauthRequest);
}
} // namespace GS
