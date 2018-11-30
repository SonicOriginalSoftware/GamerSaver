#include <QFile>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <oauth.h>

struct GS::OAuth2::Credentials
{
	QString client_id{""};
	QString auth_uri{""};
	QString token_uri{""};
	QList<QString> redirect_uris{""};
};

void GS::OAuth2::SetClientId()
{
	QFile credentialFile{":/documents/credentials.json"};
	QJsonParseError jsonErr{};
	credentialFile.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::Text);
	QJsonDocument credentialJson{QJsonDocument::fromJson(credentialFile.readAll(),
														 &jsonErr)};
	credentialFile.close();
	if (jsonErr.error != QJsonParseError::NoError) return;
	QJsonObject json{credentialJson.object()};
	QString client_id = json["client_id"].toString();
	QString auth_uri = json["auth_uri"].toString();
	QString token_uri = json["token_uri"].toString();
	QVariantList redirect_uris = json["redirect_uris"].toArray().toVariantList();
}

GS::OAuth2::OAuth2()
{
}
