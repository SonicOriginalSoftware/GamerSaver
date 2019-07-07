#pragma once

class QString;
class QStringList;

namespace GS
{
class OAuth2
{
  bool errored{false};

public:
  explicit OAuth2();
  ~OAuth2();

  bool Errored() const;

  struct Credentials
  {
    QString *client_id{nullptr};
    QString *auth_uri{nullptr};
    QString *token_uri{nullptr};
    QStringList *redirect_uris{nullptr};
  };
  Credentials credentials;
};
} // namespace GS
