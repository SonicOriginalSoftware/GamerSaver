#pragma once

class QTcpServer;
class QTcpSocket;
class QNetworkAccessManager;
class QNetworkRequest;
class QMessageBox;
class QStatusBar;

namespace GS
{
class OAuth2
{

  enum ReturnCodes
  {
    OK,
    NETWORK_ERR,
    CANCELLED,
    UNHANDLED,
    SERVER_ERR
  };
  struct Credentials
  {
    static const QString client_id;
    static const QString redirect_uri;
  };
  struct OAuthParameters
  {
    static const QString TokenResponseType;
    static const QString Scope;
    static const QString Prompt;
    static const QString State;
  };
  struct DiscoveryDocKeyNames
  {
    static const QString AuthorizationEndpointKeyName;
    static const QString UserInfoEndpointKeyName;
  };
  struct OAuthEndpoints
  {
    static const QString GoogleDiscoveryDoc;
    QString GoogleAuthServer;
    QString GoogleUserInfo;
  };
  struct UserProfile
  {
    static const QString defaultName;
    static const QString defaultURL;
    QString pictureURL;
    QString name;
  };
  struct Tokens { QString accessToken; };

  static const QByteArray responseHTML;
  static const QByteArray okResponse;
  const QString profilePictureFileName;

  UserProfile profile;
	OAuthEndpoints endpoints;
  Tokens tokens;

  QStatusBar *statusBar;
  QNetworkAccessManager *qnam;
  QMessageBox *dialog;

  bool loggedIn{false};
  bool errored{true};
  const int messageTimeout{5000};

  void shutdownServer(QTcpServer&) const;
  ReturnCodes get(const QNetworkRequest&, QByteArray&) const;
  ReturnCodes populateGoogleEndpoints();
  QUrl buildPromptURL(const int&) const;
  ReturnCodes awaitAndRespondOnLoopback(QTcpServer&, const QByteArray&) const;
  ReturnCodes awaitAndRespondOnLoopback(QTcpServer&, const QByteArray&, QByteArray&) const;
  ReturnCodes promptForConsent(QByteArray&) const;

public:
  explicit OAuth2(QStatusBar*);
  ~OAuth2();

  bool LoggedIn() const;
  bool Errored() const;
  void LogIn();
  void LogOut();
  const QString& ProfileName() const;
  static const QString& DefaultPictureURL();
  static const QString& DefaultName();
  const QString& ProfilePictureURL();
};
} // namespace GS
