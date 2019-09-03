#pragma once
#include <QString>

// class QByteArray;
class QEventLoop;

namespace OAuth
{
enum class ConsentReturnCodes {
  OK,
  CANCELLED,
  SERVER_ERR,
  CONSENT_ERR,
  CONSENT_DENIED,
  NON_UNIQUE_REQUEST
};

struct ConsentResponse {
  const QString accessToken;
  const ConsentReturnCodes error;
  ConsentResponse(const QString&, const ConsentReturnCodes&);
};

class OAuthLoopbackServer {
  static const int listenPort{8080};
  static const QByteArray okResponse;
  static const QByteArray getConsentHTML;

public:
  static int GetListenPort();
  static ConsentResponse ListenForConsent(const QString&, QEventLoop&, const QString&);
};
} // namespace OAuth
