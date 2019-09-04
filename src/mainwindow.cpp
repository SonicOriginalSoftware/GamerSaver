#include "mainwindow.h"
#include "game.h"
#include "gsoauth.h"
#include <QMenuBar>

namespace GS {
MainWindow::MainWindow() {
  setWindowTitle("GamerSaver");
  setCentralWidget(&centralWidget);

  QMenu *themeMenu{menuBar()->addMenu("Settings")->addMenu("Theme")};
  QActionGroup* themeActions = new QActionGroup{themeMenu};
  themeMenu->addActions(QList<QAction*>{
    new QAction{"Dark Theme", themeActions},
    new QAction{"Light Theme", themeActions},
    new QAction{"Custom Theme", themeActions}}
  );
  for(const auto& eachAction : themeMenu->actions()) {
    eachAction->setCheckable(true);
  }
  themeMenu->actions().first()->setChecked(true);

  QMenu *helpMenu{menuBar()->addMenu("About")};
  helpMenu->addActions(QList<QAction*>{
    new QAction{"Help"},
    new QAction{"Check for Update..."}
  });

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
