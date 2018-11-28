#pragma once
#include <QMainWindow>

// Forward declarations
class QStringListModel;
class QGridLayout;
class QPushButton;
class QComboBox;
class QListView;

namespace GS
{
class MainWindow : public QMainWindow
{
	Q_OBJECT

	QHash<QString, QStringList>*	gameMap{nullptr};
	QStringListModel*				saveLM{nullptr};
	QStringListModel*				gameLM{nullptr};

	QWidget*						gridLayoutWidget{nullptr};
	QGridLayout*					gridLayout{nullptr};
	QPushButton*					loginBtn{nullptr};
	QComboBox*						gameSelector{nullptr};
	QPushButton*					refreshBtn{nullptr};
	QListView*						saveList{nullptr};

private slots:
	void on_gameSelector_currentTextChanged	(const QString&) const;
	void on_refreshBtn_clicked				(const bool&) const;

public:
	explicit MainWindow();
	~MainWindow();
};
}
