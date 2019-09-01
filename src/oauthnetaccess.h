#pragma once
#include <QtNetwork/QNetworkAccessManager>

class QString;
class QByteArray;
class QEventLoop;
class QNetworkRequest;

namespace GS {
enum class NetAccessReturnCodes {
  OK,
  NETWORK_ERR
};

class OAuthNetAccess {
  QNetworkAccessManager qnam;
  QEventLoop& _loop;

  NetAccessReturnCodes get(const QNetworkRequest&, QByteArray&);

public:
  OAuthNetAccess(QEventLoop&);
  NetAccessReturnCodes Get(const QString&, QByteArray&);
  NetAccessReturnCodes Get(const QString&, const QByteArray&, QByteArray&);
  static bool SSLSupported();
  static QString GetSSLBuildVersion();
};
} // namespace GS
