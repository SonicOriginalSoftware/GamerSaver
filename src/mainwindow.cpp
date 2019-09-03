#include "mainwindow.h"
#include "game.h"
#include "gsoauth.h"

namespace GS {

MainWindow::MainWindow() {
  refreshBtn.setText("Refresh");

  setWindowTitle("GamerSaver");
  setCentralWidget(&gridLayoutWidget);
  gridLayoutWidget.setLayout(&gridLayout);

  gridLayout.setContentsMargins(4, 4, 4, 4);
  gridLayout.addWidget(&gameSelector, 0, 0, 1, 4);
  gridLayout.addWidget(&refreshBtn, 0, 4, 1, 1);
  gridLayout.addWidget(&saveList, 1, 0, 1, 5);
  // FIXME Create a method to add the loginBtn of the GSOAuth to the gridLayout
  // gridLayout.addWidget(&loginBtn, 2, 0, 1, 5);

  QObject::connect(&refreshBtn, &QPushButton::clicked, this, &MainWindow::refresh);
  QObject::connect(&gameSelector, &QComboBox::currentTextChanged,
    [=](const QString& gameName) { saveLM.setStringList(games[gameName]); });

  refresh();
  saveList.setModel(&saveLM);
  gameSelector.setModel(&gameLM);

  // gsOAuth.Logout();
}

void MainWindow::refresh(const bool&) {
  games = Game::BuildGames();
  gameLM.setStringList(games.keys());

  if (gameLM.stringList().length() > 0) gameSelector.setCurrentText(gameLM.stringList().first());
}
} // namespace GS
