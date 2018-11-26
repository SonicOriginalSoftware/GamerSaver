#pragma once
#include <QNetworkAccessManager>

class QString;
class QByteArray;
class QEventLoop;
class QNetworkRequest;

namespace GS {
class OAuthNetAccess {
  QNetworkAccessManager qnam;
  QEventLoop &loop;

  QByteArray get(const QNetworkRequest &);

public:
  OAuthNetAccess(QEventLoop &);
  QByteArray Get(const QString &);
  QByteArray Get(const QString &, const QString &);
  static bool SSLSupported();
  static QString GetSSLBuildVersion();
};
} // namespace GS
