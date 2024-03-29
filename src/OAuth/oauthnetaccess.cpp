#include "oauthnetaccess.h"
#include <QByteArray>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslSocket>
#include <QString>

namespace GS {
OAuthNetAccess::OAuthNetAccess(QEventLoop& loop) : loop{loop} {
  QObject::connect(&qnam, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
}

bool OAuthNetAccess::SSLSupported() {return QSslSocket::supportsSsl();}

QString OAuthNetAccess::GetSSLBuildVersion() {return QSslSocket::sslLibraryBuildVersionString();}

QByteArray OAuthNetAccess::get(const QNetworkRequest &request) {
  QNetworkReply *reply{qnam.get(request)};
  QByteArray response{""};
  if (loop.exec() == 0 && reply->error() == QNetworkReply::NoError)
    response = reply->readAll();

  delete reply;
  return response;
}

QByteArray OAuthNetAccess::Get(const QString &requestUrl) {
  return get(QNetworkRequest(QUrl(requestUrl)));
}

QByteArray OAuthNetAccess::Get(const QString &requestUrl, const QString &token) {
  QNetworkRequest oauthRequest{QUrl{requestUrl}};
  oauthRequest.setRawHeader("Authorization", "Bearer " + token.toUtf8());
  return get(oauthRequest);
}
} // namespace GS
