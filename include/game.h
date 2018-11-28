#pragma once

// Forward declarations
class QString;
class QStringList;
template <class key, class value>
class QHash;
class QFileSystemWatcher;

namespace GS {
///
/// \brief The Game class
///
class Game
{
public:
	///
	/// \brief Really cool function! (TODO!)
	///
	/// TODO
	/// \param ???
	/// \return A hashtable of string and list of strings
	///
	static QHash<QString, QStringList> UpdateInstalledGames();
};
}

