#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <game.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
										  gameMap{Game::GetInstalledGames()},
										  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	gameLM = new QStringListModel(gameMap.keys(), ui->gameSelector);
	ui->gameSelector->setModel(gameLM);

	saveLM = new QStringListModel(gameMap[ui->gameSelector->currentText()],
								  ui->saveList);
	ui->saveList->setModel(saveLM);
}

MainWindow::~MainWindow() { delete ui; delete gameLM; delete saveLM; }

void MainWindow::on_gameSelector_currentTextChanged(const QString &gameName)
{
	saveLM->setStringList(gameMap[gameName]);
}
