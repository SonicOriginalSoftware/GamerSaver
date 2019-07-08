#pragma once
#include <QObject>

class QString;
class QTcpServer;
class QTcpSocket;
class QNetworkAccessManager;

namespace GS
{
class OAuth2 : public QObject
{
  Q_OBJECT

  struct Credentials;
  struct Tokens;
  struct OAuthParameters;
  struct OAuthEndpoints;
  struct UserProfile;
  struct OAuthResponse;

  Credentials *credentials;
  Tokens *tokens;
  UserProfile *profile;

  bool errored{false};
  const int timeout{5000};

  void shutdownServer(QTcpServer&) const;
  void shutdownServer(QTcpServer&, QTcpSocket*) const;

public:
  enum State {
    UNDEFINED,
    SERVER_ERROR,
    LOGGED_IN,
    DENIED_PERMISSION,
    UNKNOWN = 999
  };

  explicit OAuth2();
  ~OAuth2();

  bool Errored() const;

  void RequestLogin(const QNetworkAccessManager&) const;
  const QString& ProfileImageURL() const;
};
} // namespace GS

