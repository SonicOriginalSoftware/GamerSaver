#pragma once
#include "oauth.h"

namespace GS
{
class GoogleOAuth : public OAuth2
{
  static const QString authorizationEndpointKeyName;
  static const QString userInfoEndpointKeyName;

public:
  QString GetAuthEndpointKeyName() const;
  QString GetUserInfoEndpointKeyName() const;
  using OAuth2::OAuth2;
};
} // namespace GS
