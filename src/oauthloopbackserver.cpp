#include "oauthloopbackserver.h"
#include <QByteArray>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>

namespace GS {
const QByteArray OAuthLoopbackServer::okResponse{
    "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html; "
    "charset=UTF-8\r\n\r\n"};

const QByteArray OAuthLoopbackServer::responseHTML{
    "<!DOCTYPE html><html "
    "lang='en'><head><title>GamerSaver</title></head><body>You may now close "
    "this window <script>const y = window.location.hash.replace('#', "
    "'').split('&');const r = {};y.forEach(entry => {const s = "
    "entry.split('=');r[s[0]] = s[1]});fetch('/', {method:'POST', "
    "headers:{'Content-Type':'application/json'}, "
    "body:JSON.stringify(r)})</script></body></html>"};

int OAuthLoopbackServer::GetListenPort() { return listenPort; }

QByteArray OAuthLoopbackServer::PromptForConsent(const QString& listenAddress, QMessageBox& dialog)
{
  // FIXME This will not work! need to listen, respond, and write to client twice
  // (Once for initial consent trigger and the second for obtainining the actual consent result
  // Or...since the second time contains the actual result...is this necessary?
  // Yes, because the server won't respond twice - this method is only fired once)
  QTcpServer server{};
  QObject::connect(&server, &QTcpServer::newConnection, &dialog, &QMessageBox::accept);
  if (!server.listen(QHostAddress(listenAddress))) return "";
  dialog.exec();

  QTcpSocket *connection{server.nextPendingConnection()};
  if (dialog.result() == QMessageBox::StandardButton::Cancel || connection == nullptr) return "";

  connection->write(okResponse + responseHTML);
  connection->flush();
  connection->waitForReadyRead();
  QByteArray response{connection->readAll()};
  connection->disconnectFromHost();
  delete connection;
  server.close();
  return response;
}
} // namespace GS

