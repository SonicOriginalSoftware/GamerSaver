#pragma once

class QString;
class QByteArray;
class QMessageBox;

namespace GS
{
class OAuthLoopbackServer
{
  static const int listenPort{8080};
  static const QByteArray okResponse;
  static const QByteArray responseHTML;
  OAuthLoopbackServer() {};

public:
  static int GetListenPort();
  static QByteArray PromptForConsent(const QString&, QMessageBox&);
};
}

