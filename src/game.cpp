#include "game.h"
#include <QSettings>
#include <QDir>

QHash<QString, QStringList> GS::Game::UpdateInstalledGames()
{
	const QSettings defaultGames(":/documents/DefaultGameList.ini", QSettings::IniFormat);
	const QSettings customGames("res/GameList.ini", QSettings::IniFormat);
	QStringList	gameList{customGames.childGroups()};
	for (const QString& gameName : defaultGames.childGroups())
	{
		if (!gameList.contains(gameName)) gameList.append(gameName);
	}

	QDir saveDir;
	QHash<QString, QStringList> returnGames;
	for (const QString& gameName : gameList) {
		QString savePath = customGames.value(gameName + "/path",
						   defaultGames.value(gameName + "/path")).toString();
		saveDir.setPath(QDir::homePath() + "/" + savePath);

		QStringList saveExtensions = customGames.value(gameName + "/ext",
					defaultGames.value(gameName + "/ext")).toStringList();
		saveDir.setNameFilters(saveExtensions);

		if (saveDir.exists() && saveDir.count() > 0)
			returnGames[gameName] = saveDir.entryList();
		saveDir.entryList();
	}
	return returnGames;
}
