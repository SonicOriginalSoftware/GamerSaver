#include "mainwindow.h"
#include "game.h"
#include "oauth.h"
#include <QStringListModel>
#include <QtGui/QIcon>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

GS::MainWindow::MainWindow() :
    gameMap{new QHash<QString, QStringList>},
    saveLM{new QStringListModel(this)},
    gameLM{new QStringListModel(gameMap->keys(), this)},
    gridLayoutWidget{new QWidget(this)},
    gridLayout{new QGridLayout(gridLayoutWidget)},
    loginBtn{new QPushButton(QIcon(":/images/ic_account_box_white_24px.svg"),
                             "Login", gridLayoutWidget)},
    gameSelector{new QComboBox(gridLayoutWidget)},
    refreshBtn{new QPushButton("Refresh", gridLayoutWidget)},
    saveList{new QListView(gridLayoutWidget)}
{
  setWindowTitle("GamerSaver");
  resize(737, 564);
  setCentralWidget(gridLayoutWidget);

  gridLayout->setContentsMargins(4, 4, 4, 4);
  loginBtn->setEnabled(false);

  setObjectName("MainWindow");
  gridLayoutWidget->setObjectName("gridLayoutWidget");
  gridLayout->setObjectName("gridLayout");
  loginBtn->setObjectName("loginBtn");
  refreshBtn->setObjectName("refreshBtn");
  gameSelector->setObjectName("gameSelector");
  saveList->setObjectName("saveList");

  gridLayout->addWidget(loginBtn,     0, 0, 1, 5);
  gridLayout->addWidget(gameSelector, 1, 0, 1, 4);
  gridLayout->addWidget(refreshBtn,   1, 4, 1, 1);
  gridLayout->addWidget(saveList,     2, 0, 1, 5);

  QMetaObject::connectSlotsByName(this);

  saveList->setModel(saveLM);
  gameSelector->setModel(gameLM);
}

GS::MainWindow::~MainWindow()
{
  delete gameMap;
  delete saveLM;
  delete gameLM;
  delete gridLayoutWidget;
}

void GS::MainWindow::on_gameSelector_currentTextChanged(
    const QString &gameName) const
{
  saveLM->setStringList((*gameMap)[gameName]);
}

void GS::MainWindow::on_refreshBtn_clicked(const bool &) const
{
  // *gameMap = GS::Game::UpdateInstalledGames();
  gameLM->setStringList(gameMap->keys());
  gameSelector->setCurrentText(gameLM->stringList().first());
}
