#pragma once

class QString;
class QByteArray;
class QEventLoop;

namespace GS
{
class OAuthLoopbackServer
{
  static const int listenPort{8080};
  static const QByteArray okResponse;
  static const QByteArray responseHTML;

public:
  static int GetListenPort();
  static QByteArray PromptForConsent(const QString&, QEventLoop&);
};
}

