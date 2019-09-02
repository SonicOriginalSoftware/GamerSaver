#pragma once
#include "oauth.h"

namespace GS
{
class GoogleOAuth : public OAuth2
{
  static const QString discoveryDocUrl;
  static const QString authorizationEndpointKeyName;
  static const QString userInfoEndpointKeyName;

public:
  explicit GoogleOAuth(const QString&, const QString&);
  static QString GetDiscoveryDocEndpoint();
  static QString GetAuthEndpointKeyName();
  static QString GetUserInfoEndpointKeyName();

  QString BuildURL(int) const;
};
} // namespace GS
