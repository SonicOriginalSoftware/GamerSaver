#include "oauthloopbackserver.h"
#include <QByteArray>
#include <QTcpServer>
#include <QTcpSocket>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>

namespace OAuth {
const QByteArray OAuthLoopbackServer::okResponse{
    "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html;"
    "charset=UTF-8\r\n\r\n"};

const QByteArray OAuthLoopbackServer::getConsentHTML{
    "<!DOCTYPE html><html lang='en'><head><title>GamerSaver</title></head>"
    "<body>You may now close this window <script>"
      "const y = window.location.hash.replace('#', '').split('&');"
      "const r = {};"
      "y.forEach(entry => {const s = entry.split('=');r[s[0]] = s[1]});"
      "fetch('/', {"
        "method:'POST',"
        "headers:{'Content-Type':'application/json'},body:JSON.stringify(r)"
      "});"
    "</script></body></html>"};

int OAuthLoopbackServer::GetListenPort() { return listenPort; }
ConsentResponse::ConsentResponse(const QString& accessToken,
                                 const ConsentReturnCodes& error)
  : accessToken{accessToken}, error{error} {}

ConsentResponse OAuthLoopbackServer::ListenForConsent(const QString& listenAddress, QEventLoop& loop, const QString& state) {
  QString accessToken{""};
  ConsentReturnCodes returnCode{ConsentReturnCodes::SERVER_ERR};

  QTcpServer server{};
  QObject::connect(&server, &QTcpServer::newConnection, &loop, &QEventLoop::quit);
  if (!server.listen(QHostAddress(listenAddress), listenPort))
    return OAuth::ConsentResponse{accessToken, returnCode};

  QByteArray response{""};
  int loopResult{loop.exec()};
  QTcpSocket *initialConnection{server.nextPendingConnection()};
  if (initialConnection != nullptr && loopResult == 0) {
    initialConnection->write(okResponse + getConsentHTML);
    initialConnection->flush();
    initialConnection->waitForReadyRead();
    response = initialConnection->readAll();
    initialConnection->disconnectFromHost();
    delete initialConnection;

    int loopResult{loop.exec()};
    QTcpSocket *consentConnection{server.nextPendingConnection()};
    if (consentConnection != nullptr && loopResult == 0) {
      consentConnection->write(okResponse);
      consentConnection->flush();
      consentConnection->waitForReadyRead();
      response = consentConnection->readAll();
      consentConnection->disconnectFromHost();
      delete consentConnection;

      QString stringResponse{response};
      int firstIndex{stringResponse.lastIndexOf("{")};
      QJsonObject responseAsJson = QJsonDocument::fromJson(
        QStringRef{&stringResponse, firstIndex, response.length() - firstIndex}.toUtf8()
      ).object();

      accessToken = responseAsJson["access_token"].toString();
      if (!responseAsJson.contains("error")) {
        returnCode = ConsentReturnCodes::OK;
      } else {
        returnCode = ConsentReturnCodes::CONSENT_ERR;
        if (responseAsJson["error"].toString() == "access_denied")
          returnCode = ConsentReturnCodes::CONSENT_DENIED;
      }
      if (responseAsJson["state"].toString() != state) {
          returnCode = ConsentReturnCodes::NON_UNIQUE_REQUEST;
      }
    }
  }
  else if (loopResult != 0) { returnCode = ConsentReturnCodes::CANCELLED; }

  server.close();
  return OAuth::ConsentResponse{accessToken, returnCode};
}
} // namespace OAuth
