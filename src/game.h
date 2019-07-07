#pragma once

template<class K, class V>
class QHash;
class QSettings;
class QString;
class QStringList;

namespace GS
{
class Game
{
  static void updateGames(QHash<QString, QStringList>&, const QSettings&);

public:
  static const QHash<QString, QStringList> BuildGames();
};
} // namespace GS

