#include "oauthloopbackserver.h"
#include <QByteArray>
#include <QTcpServer>
#include <QTcpSocket>
#include <QEventLoop>

namespace GS {
const QByteArray OAuthLoopbackServer::okResponse{
    "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html;"
    "charset=UTF-8\r\n\r\n"};

const QByteArray OAuthLoopbackServer::responseHTML{
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
  // FIXME This will not work! need to listen, respond, and write to client twice
  // (Once for initial consent trigger and the second for obtainining the actual consent result
  // Or...since the second time contains the actual result...is this necessary?
  // Yes, because the server won't respond twice - this method is only fired once)

  ServerReturnCodes returnCode{ServerReturnCodes::SERVER_ERR};
  QTcpServer server{};
  QObject::connect(&server, &QTcpServer::newConnection, &loop, &QEventLoop::quit);

  if (!server.listen(QHostAddress(listenAddress))) return returnCode;
  int loopResult{loop.exec()};

  QTcpSocket *connection{server.nextPendingConnection()};
  if (loopResult == -1) { returnCode = ServerReturnCodes::CANCELLED; }
  else if (connection != nullptr && loopResult == 0) {
    connection->write(okResponse + responseHTML);
    connection->flush();
    connection->waitForReadyRead();
    response = connection->readAll();
    connection->disconnectFromHost();
    delete connection;
  }

  server.close();
  return ServerReturnCodes::OK;
}
} // namespace GS
