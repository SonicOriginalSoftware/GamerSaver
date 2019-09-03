#pragma once
#include "../oauth.h"

namespace OAuth {
class GoogleOAuth : protected OAuth::OAuth2
{
protected:
  static const QString DiscoveryDocUrl;
  static const QString AuthEndpointKey;
  static const QString UserInfoEndpointKey;

  QString BuildURL(int) const;
};
} // namespace OAuth
