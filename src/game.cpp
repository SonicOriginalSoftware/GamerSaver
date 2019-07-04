#include "game.h"
#include <QDir>

void GS::Game::UpdateInstalledGames() {
  const QString gameDir = QDir::homePath() + QDir::separator() + "My Games";
  QStringList gamesInGameDir{};
  for (const auto& eachFolder : QDir(gameDir).entryList())
  {
    gamesInGameDir.append(eachFolder);
  }
  // TODO
  return;
}
