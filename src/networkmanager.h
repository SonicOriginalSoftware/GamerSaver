#pragma once

class QNetworkAccessManager;

class NetworkManager {
  QNetworkAccessManager *qnam;

  bool ResponseHandler();

public:
  NetworkManager();
  ~NetworkManager();
};
