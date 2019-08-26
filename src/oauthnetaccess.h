#pragma once

class QString;
class QByteArray;
class QMessageBox;
class QNetworkRequest;
class QNetworkAccessManager;

namespace GS {
class OAuthNetAccess {
  QNetworkAccessManager &qnam;
  QByteArray get(const QNetworkRequest&, QMessageBox&) const;

public:
  OAuthNetAccess(QNetworkAccessManager &qnam);

  QByteArray Get(const QString&, QMessageBox&) const;
  QByteArray Get(const QString&, QMessageBox&, const QByteArray&) const;
  static bool SSLSupported();
  static QString GetSSLBuildVersion();
  bool NetworkConnected() const;
};
} // namespace GS

