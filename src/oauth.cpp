#include <QString>
#include <QStringList>
#include <QResource>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include "oauth.h"

GS::OAuth2::OAuth2()
{
  QString credentialFilePath{":/res/apikey.json"};
  QResource credentialResFile{credentialFilePath};
  if (!credentialResFile.isValid())
  {
    errored = true;
  }
  QFile credentialFile{credentialFilePath};
  if (!credentialFile.open(QFile::ReadOnly))
  {
    errored = true;
  }
  QByteArray jsonData{credentialFile.readAll()};
  credentialFile.close();

  QJsonParseError jsonErr{};
  QJsonDocument credentialJson{QJsonDocument::fromJson(jsonData, &jsonErr)};
  if (jsonErr.error != QJsonParseError::NoError)
  {
    errored = true;
  }

  QJsonObject json{credentialJson.object()};
  json = json["installed"].toObject();

  // *credentials.client_id = json["client_id"].toString();
  // *credentials.auth_uri = json["auth_uri"].toString();
  // *credentials.token_uri = json["token_uri"].toString();
  // *credentials.redirect_uris = json["redirect_uris"].toVariant().toStringList();
}

GS::OAuth2::~OAuth2() {}

bool GS::OAuth2::Errored() const { return errored; }
