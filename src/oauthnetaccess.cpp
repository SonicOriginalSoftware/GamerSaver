#include "oauthnetaccess.h"
#include <QByteArray>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslSocket>
#include <QString>

namespace GS {
OAuthNetAccess::OAuthNetAccess(QEventLoop& loop) : _loop{loop} {
  QObject::connect(&qnam, &QNetworkAccessManager::finished, &_loop, &QEventLoop::quit);
}

bool OAuthNetAccess::SSLSupported() {return QSslSocket::supportsSsl();}

QString OAuthNetAccess::GetSSLBuildVersion() {return QSslSocket::sslLibraryBuildVersionString();}

NetAccessReturnCodes OAuthNetAccess::get(const QNetworkRequest &request, QByteArray& response) {
  QNetworkReply *reply{qnam.get(request)};
  NetAccessReturnCodes returnCode{NetAccessReturnCodes::NETWORK_ERR};
  if (_loop.exec() == 0 && reply->error() == QNetworkReply::NoError)
  {
    response = reply->readAll();
    returnCode = NetAccessReturnCodes::OK;
  }

  delete reply;
  return returnCode;
}

NetAccessReturnCodes OAuthNetAccess::Get(const QString &requestUrl, QByteArray& response) {
  return get(QNetworkRequest(QUrl(requestUrl)), response);
}

NetAccessReturnCodes OAuthNetAccess::Get(const QString &requestUrl, const QByteArray &token, QByteArray& response) {
  QNetworkRequest oauthRequest{QUrl{requestUrl}};
  oauthRequest.setRawHeader("Authorization", "Bearer " + token);
  return get(oauthRequest, response);
}
} // namespace GS
