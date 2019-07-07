#include "oauth.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QResource>
#include <QString>
#include <QStringList>
#include <QVariant>

struct GS::OAuth2::Credentials {
  QString client_id;
  QString auth_uri;
  QString token_uri;
  QStringList redirect_uris;
};

GS::OAuth2::OAuth2() : credentials(new Credentials()) {
  QFile credentialFile{":/res/apikey.json"};
  if (!credentialFile.open(QFile::ReadOnly)) {
    errored = true;
    return;
  }
  QByteArray jsonData{credentialFile.readAll()};
  credentialFile.close();

  QJsonParseError jsonErr{};
  QJsonDocument credentialJson{QJsonDocument::fromJson(jsonData, &jsonErr)};
  if (jsonErr.error != QJsonParseError::NoError) {
    errored = true;
    return;
  }

  QJsonObject json{credentialJson.object()};
  json = json["installed"].toObject();

  credentials->client_id = json["client_id"].toString();
  credentials->auth_uri = json["auth_uri"].toString();
  credentials->token_uri = json["token_uri"].toString();
  credentials->redirect_uris = json["redirect_uris"].toVariant().toStringList();
}

GS::OAuth2::~OAuth2() { delete credentials; }

bool GS::OAuth2::Errored() const { return errored; }
const QString &GS::OAuth2::ClientID() const { return credentials->client_id; }
const QString &GS::OAuth2::AuthURI() const { return credentials->auth_uri; }
const QString &GS::OAuth2::TokenURI() const { return credentials->token_uri; }
const QStringList &GS::OAuth2::RedirectURIs() const { return credentials->redirect_uris; }

