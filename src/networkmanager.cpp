#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "networkmanager.h"

NetworkManager::NetworkManager() : qnam{new QNetworkAccessManager()}
{
	QNetworkAccessManager::connect(qnam, qnam->finished(QNetworkReply*),
								   this, ResponseHandler());
}

NetworkManager::~NetworkManager() { delete qnam; }
