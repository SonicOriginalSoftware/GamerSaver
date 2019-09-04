#pragma once
#include "OAuth/Google/googleoauth.h"
#include "OAuth/oauthnetaccess.h"
#include <QEventLoop>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>

namespace GS {
class MainWindow;

class GSOAuth : OAuth::GoogleOAuth {
  static const QString loginBtnDefaultValue;
  static const QString defaultProfilePicPath;
  const QString profilePicPath;

  QEventLoop loop{};
  QMessageBox dialog{};
  QStatusBar statusBar{};
  OAuthNetAccess oauthNetAccess{loop};

public:
  QPushButton loginBtn{};

  explicit GSOAuth(std::function<void(QStatusBar *)>,
                   std::function<void(QPushButton *)>);
  void Login();
  void Logout();
};
} // namespace GS
