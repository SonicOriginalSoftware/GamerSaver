#pragma once
#include "oauth.h"

namespace GS
{
class GoogleOAuth : public OAuth2
{
  static const QString authorizationEndpointKeyName;
  static const QString userInfoEndpointKeyName;

public:
  GoogleOAuth(const QString&, const QString&);
  QString GetAuthEndpointKeyName() const;
  QString GetUserInfoEndpointKeyName() const;
};
} // namespace GS

