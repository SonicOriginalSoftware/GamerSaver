#pragma once

template<class K, class V> class QHash;
class QString;
class QStringList;

namespace GS {

class Game {
public:
  static QStringList GetGames();
  static QStringList GetSaves(const QString&);
};
} // namespace GS
