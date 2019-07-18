#pragma once
#include <QMainWindow>

class QListView;
class QGridLayout;
class QPushButton;
class QComboBox;
class QStringListModel;

namespace GS
{
class OAuth2;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  QHash<QString, QStringList> games;

  QStringListModel *gameLM{nullptr};
  QStringListModel *saveLM{nullptr};
  QWidget *gridLayoutWidget{nullptr};
  QGridLayout *gridLayout{nullptr};
  QPushButton *loginBtn{nullptr};
  QPushButton *refreshBtn{nullptr};
  QComboBox *gameSelector{nullptr};
  QListView *saveList{nullptr};

  OAuth2 *oauth{nullptr};

private slots:
  void on_gameSelector_currentTextChanged(const QString &) const;
  void on_refreshBtn_clicked(const bool &);
  void on_loginBtn_clicked(const bool &) const;

public:
  explicit MainWindow();
  ~MainWindow();
};
} // namespace GS

