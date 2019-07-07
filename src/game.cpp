#include "game.h"
#include <QDir>
#include <iostream>

QStringList GS::Game::GetGames() {
  QStringList installedGames{};
  const QString gameDir = QDir::homePath() + QDir::separator() + "My Games";
  for (const auto &eachFolder : QDir(gameDir).entryList(QDir::Readable | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Unsorted)) {
    installedGames.append(eachFolder);
  }
  return installedGames;
}

QStringList GS::Game::GetSaves(const QString &game) {
  QStringList saveList{};
  const QString gameDir = QDir::homePath() + QDir::separator() + "My Games" + QDir::separator() + game;
  for (const auto &eachFolder : QDir(gameDir).entryList(QDir::Readable | QDir::Files, QDir::Unsorted)) {
    saveList.append(eachFolder);
  }
  return saveList;
}
