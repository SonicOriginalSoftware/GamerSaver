#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

#include <mainwindow.h>
#include <game.h>
#include <QStringListModel>

GS::MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	gameMap{new QHash<QString, QStringList>{GS::Game::UpdateInstalledGames()}},
	saveLM{new QStringListModel(this)},
	gameLM{new QStringListModel(gameMap->keys(), this)},
	gridLayoutWidget{new QWidget(this)},
	gridLayout{new QGridLayout(gridLayoutWidget)},
	loginBtn{new QPushButton(gridLayoutWidget)},
	gameSelector{new QComboBox(gridLayoutWidget)},
	refreshBtn{new QPushButton(gridLayoutWidget)},
	saveList{new QListView(gridLayoutWidget)}
{
	if (objectName().isEmpty()) setObjectName("MainWindow");
	resize(737, 564);
	gridLayoutWidget->setObjectName("gridLayoutWidget");

	gridLayout->setSpacing(6);
	gridLayout->setObjectName("gridLayout");
	gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
	gridLayout->setContentsMargins(4, 4, 4, 4);

	loginBtn->setObjectName("loginBtn");
	loginBtn->setEnabled(false);
	loginBtn->setIcon(QIcon(":/images/ic_account_box_white_24px.svg"));
	loginBtn->setIconSize(QSize(32, 32));

	loginBtn->setText(QApplication::translate("MainWindow", "Login", nullptr));
	refreshBtn->setText(QApplication::translate("MainWindow", "Refresh", nullptr));

	refreshBtn->setObjectName("refreshBtn");
	gameSelector->setObjectName("gameSelector");
	saveList->setObjectName("saveList");

	gridLayout->addWidget(loginBtn, 0, 0, 1, 4);
	gridLayout->addWidget(gameSelector, 1, 0, 1, 3);
	gridLayout->addWidget(refreshBtn, 1, 3, 1, 1);
	gridLayout->addWidget(saveList, 2, 0, 1, 4);

	setCentralWidget(gridLayoutWidget);
	QMetaObject::connectSlotsByName(this);

	saveList->setModel(saveLM);
	gameSelector->setModel(gameLM);
	setWindowTitle(QApplication::translate("MainWindow", "GamerSaver", nullptr));
}

GS::MainWindow::~MainWindow()
{
	delete gameMap;
	delete gridLayoutWidget;
}

void GS::MainWindow::on_gameSelector_currentTextChanged(const QString &gameName) const
{
	saveLM->setStringList((*gameMap)[gameName]);
}
