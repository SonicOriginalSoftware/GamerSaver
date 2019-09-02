#include "oauthloopbackserver.h"
#include <QByteArray>
#include <QTcpServer>
#include <QTcpSocket>
#include <QEventLoop>

namespace GS {
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

ServerReturnCodes OAuthLoopbackServer::ListenForConsent(const QString& listenAddress, QEventLoop& loop, QByteArray& response)
{
  QTcpServer server{};
  ServerReturnCodes returnCode{ServerReturnCodes::SERVER_ERR};
  QObject::connect(&server, &QTcpServer::newConnection, &loop, &QEventLoop::quit);
  if (!server.listen(QHostAddress(listenAddress), listenPort))
    return ServerReturnCodes::SERVER_ERR;

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

      returnCode = ServerReturnCodes::OK;
    }
  }
  else if (loopResult != 0) { returnCode = ServerReturnCodes::CANCELLED; }

  server.close();
  return returnCode;
}
} // namespace GS
