#pragma once

// Forward declarations
class QString;
class QStringList;
template <class key, class value>
class QHash;
class QFileSystemWatcher;

namespace GS {
class Game
{
public:
	static QHash<QString, QStringList> UpdateInstalledGames();
};
}

