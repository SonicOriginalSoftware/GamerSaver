#include "gsoauth.h"
#include "OAuth/oauthloopbackserver.h"
#include "appmanifest.h"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QFile>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStatusBar>
#include <QString>
#include <QUrl>

namespace OAuth {
const QString OAuth2::OAuthParameters::ClientId{GS::AppManifest::ClientID};
const QString OAuth2::OAuthParameters::RedirectUri{
    GS::AppManifest::RedirectURI};
const QString OAuth2::OAuthParameters::Scope{QUrl::toPercentEncoding(
    "https://www.googleapis.com/auth/drive.file profile")};
} // namespace OAuth

namespace GS {
const QString GSOAuth::loginBtnDefaultValue{"Login"};
const QString GSOAuth::defaultProfilePictureFilePath{
    ":/res/login_placeholder.svg"};

GSOAuth::GSOAuth(QStatusBar *statusbar)
    : profilePictureFilePath{QCoreApplication::applicationDirPath().toUtf8() +
                             "profilePic.jpg"},
      statusBar{statusbar} {
  dialog.setIcon(QMessageBox::Icon::NoIcon);
  dialog.setWindowTitle("GamerSaver - Waiting...");
  dialog.setText("Press Cancel to abort the request");
  dialog.setStandardButtons(QMessageBox::Cancel);

  loginBtn.setCheckable(true);
  loginBtn.setEnabled(false);
  loginBtn.setObjectName("loginBtn");

  QObject::connect(&dialog, &QMessageBox::finished, &loop, &QEventLoop::exit);
  QObject::connect(
      &loginBtn, &QPushButton::clicked,
      [=](const bool &unchecked) { unchecked ? Logout() : Login(); });

  if (!OAuthNetAccess::SSLSupported()) {
    statusBar->showMessage("No SSL library detected. Install " +
                           oauthNetAccess.GetSSLBuildVersion());
    return;
  }

  statusBar->showMessage("Requesting Google Discovery Doc from " +
                         GoogleOAuth::DiscoveryDocUrl);
  dialog.show();
  QByteArray discoveryDoc{oauthNetAccess.Get(GoogleOAuth::DiscoveryDocUrl)};
  dialog.hide();

  if (discoveryDoc == "") {
    statusBar->showMessage("Endpoints could not be retrieved. Try again!");
    return;
  }

  QJsonObject discoveryDocObject{
      QJsonDocument::fromJson(discoveryDoc).object()};

  endpoints.auth =
      discoveryDocObject[GoogleOAuth::AuthorizationEndpointKeyName].toString();
  endpoints.userInfo =
      discoveryDocObject[GoogleOAuth::UserInfoEndpointKeyName].toString();

  statusBar->showMessage("Endpoints retrieved. Log in when ready!");
  loginBtn.setEnabled(true);
}

void GSOAuth::Logout() {
  ClearUser();
  QFile::remove(profilePictureFilePath);
  loginBtn.setText(loginBtnDefaultValue);
  loginBtn.setIcon(QIcon{defaultProfilePictureFilePath});
  loginBtn.setChecked(false);
  statusBar->showMessage("Logged out");
  return;
}

void GSOAuth::Login() {
  statusBar->showMessage("Awaiting user consent...");
  if (!QDesktopServices::openUrl(
          QUrl{BuildURL(OAuth::OAuthLoopbackServer::GetListenPort())})) {
    statusBar->showMessage("Could not open URL in desktop program!");
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
    statusBar->showMessage("Request was cancelled!");
    return;
  case OAuth::ConsentReturnCodes::SERVER_ERR:
    statusBar->showMessage("An error occurred in the server!");
    return;
  case OAuth::ConsentReturnCodes::CONSENT_ERR:
    statusBar->showMessage("An error occurred in handling the consent!");
    return;
  case OAuth::ConsentReturnCodes::CONSENT_DENIED:
    statusBar->showMessage("Consent was denied!");
    return;
  case OAuth::ConsentReturnCodes::NON_UNIQUE_REQUEST:
    statusBar->showMessage("Request was not unique!");
    return;
  }

  dialog.show();
  QByteArray userInfo{
      oauthNetAccess.Get(endpoints.userInfo, tokens.accessToken)};
  dialog.hide();
  if (userInfo == "") {
    statusBar->showMessage("User info invalid!");
    return;
  }

  QJsonObject userInfoObject{QJsonDocument::fromJson(userInfo).object()};
  if (userInfoObject.isEmpty())
    return;

  profile.name = userInfoObject["name"].toString().toUtf8();
  profile.pictureURL = userInfoObject.contains("picture")
                           ? userInfoObject["picture"].toString().toUtf8()
                           : QString{""};
  loginBtn.setChecked(true);
  loginBtn.setText(profile.name);
  statusBar->showMessage("Logged in");

  if (profile.pictureURL == "")
    return;

  dialog.show();
  QByteArray pic{oauthNetAccess.Get(profile.pictureURL, tokens.accessToken)};
  dialog.hide();
  if (pic == "") {
    loginBtn.setIcon(QIcon{defaultProfilePictureFilePath});
    statusBar->showMessage("Profile picture invalid!");
    return;
  }

  QFile profilePictureFile{profilePictureFilePath};
  if (!profilePictureFile.open(QFile::WriteOnly)) {
    statusBar->showMessage("Could not open profile picture file!");
    return;
  } else if (profilePictureFile.write(pic) != -1) {
    loginBtn.setIcon(QIcon{profilePictureFilePath});
  }
}
} // namespace GS
