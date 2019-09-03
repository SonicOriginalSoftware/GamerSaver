#pragma once
#include "OAuth/Google/googleoauth.h"
#include "OAuth/oauthnetaccess.h"
#include <QEventLoop>
#include <QMessageBox>
#include <QPushButton>

class QStatusBar;

namespace GS
{
class GSOAuth : OAuth::GoogleOAuth
{
  static const QString loginBtnDefaultValue;
  static const QString defaultProfilePictureFilePath;
  const QString profilePictureFilePath;

  QEventLoop loop{};
  QMessageBox dialog{};
  QPushButton loginBtn{};
  QStatusBar* statusBar;
  OAuthNetAccess oauthNetAccess{loop};

public:
  explicit GSOAuth(QStatusBar*);
  void Login();
  void Logout();
};
} // namespace GS
