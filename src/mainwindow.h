#pragma once
#include <QMainWindow>

namespace GS
{
class OAuth2;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  QHash<QString, QStringList> games;

  QStringListModel gameLM{};
  QStringListModel saveLM{};
  QWidget gridLayoutWidget{};
  QGridLayout gridLayout{};
  QPushButton loginBtn{};
  QPushButton refreshBtn{};
  QComboBox gameSelector{};
  QListView saveList{};

  OAuth2& _oauth;

private slots:
  void on_gameSelector_currentTextChanged(const QString &);
  void on_refreshBtn_clicked(const bool &);
  void on_loginBtn_clicked(const bool &);

public:
  explicit MainWindow(OAuth2&);
};
} // namespace GS
