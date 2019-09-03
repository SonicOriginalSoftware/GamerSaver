#include "oauth.h"
#include <QCryptographicHash>
#include <QUuid>

namespace OAuth {
const QString OAuth2::OAuthParameters::State{
    QCryptographicHash::hash(
        QUuid::createUuid().toByteArray(QUuid::WithoutBraces),
        QCryptographicHash::Sha3_256)
        .toBase64(QByteArray::Base64UrlEncoding |
                  QByteArray::OmitTrailingEquals)};

void OAuth2::ClearUser() {
  profile.name = "";
  profile.pictureURL = "";
  tokens.accessToken = "";
}

} // namespace OAuth
