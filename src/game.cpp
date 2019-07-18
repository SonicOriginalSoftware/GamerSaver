#include "game.h"
#include <QCoreApplication>
#include <QDir>
#include <QSettings>

void GS::Game::updateGames(QHash<QString, QStringList> &games, const QSettings &ini)
{
  for (const auto &eachGame : ini.childGroups())
  {
    QString gamePath{ini.value(eachGame + "/path").toString()};
    QDir gameDir{QDir::homePath() + QDir::separator() + gamePath};
    if (!gameDir.exists())
      continue;

    QString gameExt{ini.value(eachGame + "/ext").toString()};
    gameDir.setNameFilters(QStringList{gameExt});
    QStringList saveList{gameDir.entryList(QDir::Readable | QDir::Files)};

    games[eachGame] = saveList;
  }
}

const QHash<QString, QStringList> GS::Game::BuildGames()
{
  QSettings defaultINI{":/res/DefaultGameList.ini", QSettings::IniFormat};
  QSettings customINI{QCoreApplication::applicationDirPath() + "/GameList.ini", QSettings::IniFormat};

  QHash<QString, QStringList> games{};
  updateGames(games, defaultINI);
  updateGames(games, customINI);

  return games;
}
