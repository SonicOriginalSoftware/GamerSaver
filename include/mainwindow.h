#pragma once
#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
//	class QStringListModel;

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_gameSelector_currentTextChanged(const QString &arg1);

private:
	const QHash<QString, QStringList> gameMap;

	Ui::MainWindow *ui;
	QStringListModel* gameLM;
	QStringListModel* saveLM;
};
