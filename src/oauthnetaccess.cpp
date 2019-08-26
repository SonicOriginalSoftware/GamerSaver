#include "oauthnetaccess.h"
#include <QByteArray>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslSocket>
#include <QString>

namespace GS {
OAuthNetAccess::OAuthNetAccess(QNetworkAccessManager &qnam) : qnam{qnam} {}

inline bool OAuthNetAccess::SSLSupported() {return QSslSocket::supportsSsl();}

inline QString OAuthNetAccess::GetSSLBuildVersion()
{
  return QSslSocket::sslLibraryBuildVersionString();
}

inline bool OAuthNetAccess::NetworkConnected() const
{
  return qnam.networkAccessible() == QNetworkAccessManager::NetworkAccessibility::Accessible;
}

QByteArray OAuthNetAccess::get(const QNetworkRequest &request, QMessageBox &dialog) const
{
  QByteArray response{""};
  QNetworkReply *reply{qnam.get(request)};
  if (dialog.exec() == QDialog::Accepted && reply->error() == QNetworkReply::NoError)
     response = reply->readAll();

  delete reply;
  return response;
}

QByteArray OAuthNetAccess::Get(const QString &requestUrl, QMessageBox &dialog) const
{
  return get(QNetworkRequest(QUrl(requestUrl)), dialog);
}

QByteArray OAuthNetAccess::Get(const QString &requestUrl, QMessageBox &dialog, const QByteArray &token) const
{
  QNetworkRequest oauthRequest{QUrl{requestUrl}};
  oauthRequest.setRawHeader("Authorization", "Bearer " + token);
  return get(oauthRequest, dialog);
}
} // namespace GS

