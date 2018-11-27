#include "game.h"
#include <QSettings>
#include <QDir>

QHash<QString, QStringList> Game::Game::GetInstalledGames()
{
	QSettings defaultGames(":/documents/DefaultGameList.ini", QSettings::IniFormat);
	QSettings customGames("GameList.ini", QSettings::IniFormat);
	QStringList gameList { customGames.childGroups() };
	for (const QString& gameName : defaultGames.childGroups())
	{
		if (!gameList.contains(gameName)) gameList.append(gameName);
	}
	QDir saveDir;
	// Build name filters from ext keys in QSettings files

	QHash<QString, QStringList> returnGames;
	for (const QString& gameName : gameList) {
		QString savePath = customGames.value(gameName + "/path",
						   defaultGames.value(gameName + "/path")).toString();
		saveDir.setPath(QDir::homePath() + "/" + savePath);

		QList<QString> saveExtensions = customGames.value(gameName + "/ext",
						 defaultGames.value(gameName + "/ext")).toStringList();
		saveDir.setNameFilters(saveExtensions);
//		saveDir.setFilter(QDir::NoDotAndDotDot);

		if (saveDir.exists() && saveDir.count() > 0)
			returnGames[gameName] = saveDir.entryList();
		saveDir.entryList();
	}
	return returnGames;
}
