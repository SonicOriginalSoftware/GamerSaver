#pragma once

class QString;
class QByteArray;
class QEventLoop;

namespace GS
{
enum class ServerReturnCodes {
  OK,
  SERVER_ERR,
  CANCELLED,
};

class OAuthLoopbackServer
{
  static const int listenPort{8080};
  static const QByteArray okResponse;
  static const QByteArray getConsentHTML;

public:
  static int GetListenPort();
  static ServerReturnCodes ListenForConsent(const QString&, QEventLoop&, QByteArray&);
};
} // namespace GS
