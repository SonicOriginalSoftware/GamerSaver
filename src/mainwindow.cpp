#include "mainwindow.h"
#include "game.h"
#include "gsoauth.h"

namespace GS {
MainWindow::MainWindow() {
  setWindowTitle("GamerSaver");
  setCentralWidget(&centralWidget);

  gridLayout.addWidget(&saveList, 1, 0, 1, 5);
  gridLayout.addWidget(&gameSelector, 0, 0, 1, 4);
  gridLayout.addWidget(&refreshBtn, 0, 4, 1, 1);

  QObject::connect(
      &gameSelector, &QComboBox::currentTextChanged,
      [=](const QString &gameName) { saveLM.setStringList(games[gameName]); });
  QObject::connect(&refreshBtn, &QPushButton::clicked, this,
                   &MainWindow::refresh);

  refresh();
  saveList.setModel(&saveLM);
  gameSelector.setModel(&gameLM);
}

void MainWindow::refresh(const bool &) {
  games = Game::BuildGames();
  gameLM.setStringList(games.keys());

  if (gameLM.stringList().length() > 0)
    gameSelector.setCurrentText(gameLM.stringList().first());
}
} // namespace GS
