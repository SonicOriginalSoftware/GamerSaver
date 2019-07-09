#include <QHash>
#include <QStringListModel>
#include <QtGui/QIcon>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtNetwork/QNetworkAccessManager>
#include "game.h"
#include "oauth.h"
#include "mainwindow.h"

GS::MainWindow::MainWindow() : games{GS::Game::BuildGames()},
                               gameLM{new QStringListModel(games.keys(), this)},
                               saveLM{new QStringListModel(this)},
                               gridLayoutWidget{new QWidget(this)},
                               gridLayout{new QGridLayout(gridLayoutWidget)},
                               loginBtn{new QPushButton(QIcon(OAuth2::DefaultPictureURL()), OAuth2::DefaultName(), gridLayoutWidget)},
                               refreshBtn{new QPushButton("Refresh", gridLayoutWidget)},
                               gameSelector{new QComboBox(gridLayoutWidget)},
                               saveList{new QListView(gridLayoutWidget)},
                               oauth{new OAuth2()}
{
  if (oauth->Errored()) this->close();

  setWindowTitle("GamerSaver");
  setObjectName("MainWindow");
  gridLayoutWidget->setObjectName("gridLayoutWidget");
  gridLayout->setObjectName("gridLayout");
  loginBtn->setObjectName("loginBtn");
  refreshBtn->setObjectName("refreshBtn");
  gameSelector->setObjectName("gameSelector");
  saveList->setObjectName("saveList");

  setCentralWidget(gridLayoutWidget);
  gridLayout->setContentsMargins(4, 4, 4, 4);
  gridLayout->addWidget(gameSelector, 0, 0, 1, 4);
  gridLayout->addWidget(refreshBtn, 0, 4, 1, 1);
  gridLayout->addWidget(saveList, 1, 0, 1, 5);
  gridLayout->addWidget(loginBtn, 2, 0, 1, 5);

  QMetaObject::connectSlotsByName(this);

  saveList->setModel(saveLM);
  gameSelector->setModel(gameLM);
}

GS::MainWindow::~MainWindow()
{
  delete saveLM;
  delete gameLM;
  delete oauth;
  delete gridLayoutWidget;
}

void GS::MainWindow::on_gameSelector_currentTextChanged(const QString &gameName) const
{
  saveLM->setStringList(games[gameName]);
}

void GS::MainWindow::on_refreshBtn_clicked(const bool &)
{
  games = GS::Game::BuildGames();
  gameLM->setStringList(games.keys());
  if (gameLM->stringList().length() > 0) gameSelector->setCurrentText(gameLM->stringList().first());
}

void GS::MainWindow::on_loginBtn_clicked(const bool &) const
{
  oauth->LoggedIn() ? oauth->LogOut() : oauth->LogIn();
  if (oauth->Errored()) return;

  loginBtn->setText(oauth->ProfileName());
  loginBtn->setIcon(QIcon{oauth->ProfilePictureURL()});
}

