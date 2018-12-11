#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

class QNetworkAccessManager;

class NetworkManager
{
	QNetworkAccessManager* qnam;

	bool ResponseHandler();
public:
	NetworkManager();
	~NetworkManager();
};

#endif // NETWORKMANAGER_H
