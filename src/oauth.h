#pragma once

class QString;
class QByteArray;
// class QTcpServer;
class QNetworkAccessManager;
class QNetworkRequest;

namespace GS
{
enum ReturnCodes
{
  OK,
  NETWORK_ERR,
  CANCELLED,
  UNHANDLED,
  SERVER_ERR,
  IO_ERR,
  CONSENT_DENIED,
  NON_UNIQUE_REQUEST,
  CONSENT_ERR
};

class OAuth2
{
public:
  const struct OAuthEndpoints
  {
    static const QString GoogleDiscoveryDoc;
    QString GoogleAuthServer;
    QString GoogleUserInfo;
  };

private:
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
  struct UserProfile
  {
    static const QString defaultName;
    static const QString defaultURL;
    QString pictureURL;
    QString name;
  };
  struct Tokens { QByteArray accessToken; };

  static const QByteArray responseHTML;
  static const QByteArray okResponse;
  const QString profilePictureFileName;

  UserProfile profile;
	OAuthEndpoints endpoints;
  Tokens tokens;

  QByteArray consentResponse;
  QJsonObject responseAsJson;
  QNetworkAccessManager& qnam;
  QTcpServer& server;

  bool supportsSSL{true};
  const int messageTimeout{5000};

  const ReturnCodes get(const QNetworkRequest&, QByteArray&) const;
  const ReturnCodes awaitAndRespondToServer();

public:
  explicit OAuth2(QNetworkAccessManager&, QTcpServer&);

  const bool SSLSupported() const;
  const void LogOut();
  const ReturnCodes PopulateGoogleEndpoints();
  const ReturnCodes PromptForConsent();
  const ReturnCodes UpdateProfilePicture() const;
  const ReturnCodes HandleConsent();
  const ReturnCodes SetUser();
  const QString& ProfileName() const;
  static const QString& DefaultPictureURL();
  static const QString& DefaultName();
  const QString& ProfilePictureURL();
};
} // namespace GS
