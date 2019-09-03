#pragma once
#include "../oauth.h"

namespace OAuth {
class GoogleOAuth : protected OAuth::OAuth2
{
protected:
  static const QString DiscoveryDocUrl;
  static const QString AuthorizationEndpointKeyName;
  static const QString UserInfoEndpointKeyName;

  QString BuildURL(int) const;
};
} // namespace OAuth
