#pragma once

// Forward Declarations
class QFile;
class QByteArray;
class QString;

namespace GS
{
class File
{
	QFile* file;

public:
	QByteArray* data;
	File(const QString&);
	~File();
};

class OAuth2
{
	struct Credentials;

public:
	Credentials *credentials;
	explicit OAuth2();
	~OAuth2();
};
}
