#pragma once

class QString;
class QStringList;

namespace GS
{
class OAuth2
{
  struct Credentials;

  bool errored{false};

public:
  Credentials *credentials;

  explicit OAuth2();
  ~OAuth2();

  bool Errored() const;
  const QString& ClientID() const;
  const QString& AuthURI() const;
  const QString& TokenURI() const;
  const QStringList& RedirectURIs() const;
};
} // namespace GS
