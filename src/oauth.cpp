#include <QFile>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <oauth.h>

GS::File::File(const QString& filePath) : file{new QFile{filePath}}
{
	file->open(QIODevice::ReadOnly | QIODevice::Text);
	data = new QByteArray{ file->readAll() };
}

GS::File::~File() { file->close(); delete data; delete file; }

struct GS::OAuth2::Credentials
{
	QString client_id{""};
	QString auth_uri{""};
	QString token_uri{""};
	QList<QString> redirect_uris{""};
};

GS::OAuth2::OAuth2() : credentials{new Credentials{}}
{
	File _credentialFile{":/documents/credentials.json"};
	QJsonParseError jsonErr{};
	QJsonDocument credentialJson{QJsonDocument::fromJson(*_credentialFile.data, &jsonErr)};
	if (jsonErr.error != QJsonParseError::NoError) return;
	QJsonObject json{credentialJson.object()};
	credentials->client_id = json["client_id"].toString();
	credentials->auth_uri = json["auth_uri"].toString();
	credentials->token_uri = json["token_uri"].toString();
	credentials->redirect_uris = json["redirect_uris"].toVariant().toStringList();
}

GS::OAuth2::~OAuth2() { delete credentials; }
