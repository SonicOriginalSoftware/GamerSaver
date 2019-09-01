#pragma once
#include "appmanifest.h"
#include "googleoauth.h"
#include "oauthnetaccess.h"
#include <QMainWindow>
#include <QStringListModel>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QListView>
#include <QMessageBox>
#include <QEventLoop>

namespace GS
{
class MainWindow : public QMainWindow
{
  static const QString loginBtnDefaultValue;
  static const QString defaultProfilePictureFilePath;
  const QString profilePictureFilePath;

  QHash<QString, QStringList> games;

  QStringListModel gameLM{};
  QStringListModel saveLM{};
  QWidget gridLayoutWidget{};
  QGridLayout gridLayout{};
  QPushButton loginBtn{};
  QPushButton refreshBtn{};
  QComboBox gameSelector{};
  QListView saveList{};
  QMessageBox dialog{};
  QEventLoop loop{};

  GoogleOAuth googleOAuth{GS::AppManifest::ClientID, GS::AppManifest::RedirectURI};
  OAuthNetAccess oauthNetAccess{loop};

  void refresh(const bool& = false);
  void login(const bool&);
  void gameSelectionChanged(const QString&);

public:
  explicit MainWindow();
};
} // namespace GS
