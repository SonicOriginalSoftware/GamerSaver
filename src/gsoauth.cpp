#include "gsoauth.h"
#include "OAuth/oauthloopbackserver.h"
#include "mainwindow.h"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QFile>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QUrl>

namespace OAuth {
const QString OAuth2::OAuthParameters::RedirectUri{"http://localhost"};
const QString OAuth2::OAuthParameters::ClientId{
    "207822922610-jpk6ice7tsbo5ml6ig2q795ph1dnohle.apps.googleusercontent.com"};
const QString OAuth2::OAuthParameters::Scope{QUrl::toPercentEncoding(
    "https://www.googleapis.com/auth/drive.file profile")};
} // namespace OAuth

namespace GS {
const QString GSOAuth::loginBtnDefaultValue{"Login"};
const QString GSOAuth::defaultProfilePicPath{":/res/login_placeholder.svg"};

GSOAuth::GSOAuth(std::function<void(QStatusBar *)> setStatusBar,
                 std::function<void(QPushButton *)> addLoginBtn)
    : profilePicPath{QCoreApplication::applicationDirPath().toUtf8() +
                     "profilePic.jpg"} {
  addLoginBtn(&loginBtn);
  statusBar.setSizeGripEnabled(false);
  setStatusBar(&statusBar);

  dialog.setIcon(QMessageBox::Icon::NoIcon);
  dialog.setWindowTitle("GamerSaver - Waiting...");
  dialog.setText("Press Cancel to abort the request");
  dialog.setStandardButtons(QMessageBox::Cancel);

  loginBtn.setCheckable(true);
  loginBtn.setEnabled(false);
  loginBtn.setObjectName("loginBtn");
  Logout();

  QObject::connect(&dialog, &QMessageBox::finished, &loop, &QEventLoop::exit);
  QObject::connect(&loginBtn, &QPushButton::clicked,
                   [=](const bool &unchecked) {
                     loginBtn.setChecked(false);
                     !unchecked ? Logout() : Login();
                   });

  if (!OAuthNetAccess::SSLSupported()) {
    statusBar.showMessage("No SSL library detected. Install " +
                          oauthNetAccess.GetSSLBuildVersion());
    return;
  }

  statusBar.showMessage("Requesting Google Discovery Doc from " +
                        GoogleOAuth::DiscoveryDocUrl);
  dialog.show();
  QByteArray doc{oauthNetAccess.Get(GoogleOAuth::DiscoveryDocUrl)};
  dialog.hide();

  if (doc == "") {
    statusBar.showMessage("Endpoints could not be retrieved. Try again!");
    return;
  }

  QJsonObject docObj{QJsonDocument::fromJson(doc).object()};

  endpoints.auth = docObj[GoogleOAuth::AuthEndpointKey].toString();
  endpoints.userInfo = docObj[GoogleOAuth::UserInfoEndpointKey].toString();

  statusBar.showMessage("Endpoints retrieved. Log in when ready!");
  loginBtn.setEnabled(true);
}

void GSOAuth::Logout() {
  ClearUser();
  QFile::remove(profilePicPath);
  loginBtn.setText(loginBtnDefaultValue);
  loginBtn.setIcon(QIcon{defaultProfilePicPath});
  statusBar.showMessage("Logged out");
  return;
}

void GSOAuth::Login() {
  statusBar.showMessage("Awaiting user consent...");
  if (!QDesktopServices::openUrl(
          QUrl{BuildURL(OAuth::OAuthLoopbackServer::GetListenPort())})) {
    statusBar.showMessage("Could not open URL in desktop program!");
    return;
  }

  dialog.show();
  OAuth::ConsentResponse consentResponse{
      OAuth::OAuthLoopbackServer::ListenForConsent(
          OAuthParameters::RedirectUri, loop, OAuthParameters::State)};
  dialog.hide();

  switch (consentResponse.error) {
  case OAuth::ConsentReturnCodes::OK:
    break;
  case OAuth::ConsentReturnCodes::CANCELLED:
    statusBar.showMessage("Request was cancelled!");
    return;
  case OAuth::ConsentReturnCodes::SERVER_ERR:
    statusBar.showMessage("An error occurred in the server!");
    return;
  case OAuth::ConsentReturnCodes::CONSENT_ERR:
    statusBar.showMessage("An error occurred in handling the consent!");
    return;
  case OAuth::ConsentReturnCodes::CONSENT_DENIED:
    statusBar.showMessage("Consent was denied!");
    return;
  case OAuth::ConsentReturnCodes::NON_UNIQUE_REQUEST:
    statusBar.showMessage("Request was not unique!");
    return;
  }

  dialog.show();
  QByteArray user{oauthNetAccess.Get(endpoints.userInfo, tokens.accessToken)};
  dialog.hide();
  if (user == "") {
    statusBar.showMessage("User info invalid!");
    return;
  }

  QJsonObject userInfoObject{QJsonDocument::fromJson(user).object()};
  profile.name = userInfoObject["name"].toString().toUtf8();
  profile.pictureURL = userInfoObject.contains("picture")
                           ? userInfoObject["picture"].toString().toUtf8()
                           : QString{""};
  loginBtn.setChecked(true);
  loginBtn.setText(profile.name);
  statusBar.showMessage("Logged in");

  if (profile.pictureURL == "")
    return;

  dialog.show();
  QByteArray pic{oauthNetAccess.Get(profile.pictureURL, tokens.accessToken)};
  dialog.hide();
  if (pic == "") {
    statusBar.showMessage("Profile picture invalid!");
    return;
  }

  QFile profilePicFile{profilePicPath};
  if (!profilePicFile.open(QFile::WriteOnly)) {
    statusBar.showMessage("Could not open profile picture file!");
    return;
  } else if (profilePicFile.write(pic) != -1) {
    loginBtn.setIcon(QIcon{profilePicPath});
  }
}
} // namespace GS
