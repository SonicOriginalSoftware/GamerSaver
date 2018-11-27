#pragma once
#include <QHash>

class Game
{
public:
	static QHash<QString, QStringList> GetInstalledGames();
};
