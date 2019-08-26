#pragma once
#include <QMainWindow>
#include <QStringListModel>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QListView>

class QMessageBox;

namespace GS
{
class OAuthNetAccess;
class OAuthLoopbackServer;
class GoogleOAuth;

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

  static const QString loginBtnDefaultValue;
  static const QString defaultProfilePictureFilePath;

  const QString profilePictureFilePath;
  QMessageBox& dialog;
  GoogleOAuth& googleOAuth;
  OAuthNetAccess& oauthNetAccess;

  void refresh();

private slots:
  void on_gameSelector_currentTextChanged(const QString &);
  void on_refreshBtn_clicked(const bool &);
  void on_loginBtn_clicked(const bool &);

public:
  explicit MainWindow(GoogleOAuth&, OAuthNetAccess&, QMessageBox&);
};
} // namespace GS

