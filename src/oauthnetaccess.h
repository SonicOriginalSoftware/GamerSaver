#pragma once
#include <QtNetwork/QNetworkAccessManager>

class QString;
class QByteArray;
class QEventLoop;
class QNetworkRequest;

namespace GS {
class OAuthNetAccess {
  QNetworkAccessManager qnam;
  QEventLoop& _loop;

  QByteArray get(const QNetworkRequest&);

public:
  OAuthNetAccess(QEventLoop&);
  QByteArray Get(const QString&);
  QByteArray Get(const QString&, const QByteArray&);
  static bool SSLSupported();
  static QString GetSSLBuildVersion();
  bool NetworkConnected() const;
};
} // namespace GS
