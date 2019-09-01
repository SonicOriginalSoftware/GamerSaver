#include "mainwindow.h"
#include "game.h"
#include "googleoauth.h"
#include "oauthloopbackserver.h"
#include "oauthnetaccess.h"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QStatusBar>

namespace GS {
const QString MainWindow::loginBtnDefaultValue{"Login"};
const QString MainWindow::defaultProfilePictureFilePath{":/res/login_placeholder.svg"};

MainWindow::MainWindow()
  : profilePictureFilePath{QCoreApplication::applicationDirPath().toUtf8()
    + "profilePic.jpg"} {
  dialog.setIcon(QMessageBox::Icon::NoIcon);
  dialog.setWindowTitle("GamerSaver - Waiting...");
  dialog.setText("Press Cancel to abort the request");
  dialog.setStandardButtons(QMessageBox::Cancel);

  QObject::connect(&dialog, &QMessageBox::finished, &loop, &QEventLoop::exit);

  refreshBtn.setText("Refresh");

  loginBtn.setCheckable(true);
  loginBtn.setText(loginBtnDefaultValue);
  loginBtn.setIcon(QIcon(defaultProfilePictureFilePath));
  loginBtn.setEnabled(false);
  loginBtn.setObjectName("loginBtn");

  setWindowTitle("GamerSaver");
  setCentralWidget(&gridLayoutWidget);
  gridLayoutWidget.setLayout(&gridLayout);

  gridLayout.setContentsMargins(4, 4, 4, 4);
  gridLayout.addWidget(&gameSelector, 0, 0, 1, 4);
  gridLayout.addWidget(&refreshBtn, 0, 4, 1, 1);
  gridLayout.addWidget(&saveList, 1, 0, 1, 5);
  gridLayout.addWidget(&loginBtn, 2, 0, 1, 5);

  QObject::connect(&refreshBtn, &QPushButton::clicked, this, &MainWindow::refresh);
  QObject::connect(&loginBtn, &QPushButton::clicked, this, &MainWindow::login);
  QObject::connect(&gameSelector, &QComboBox::currentTextChanged,
      [=](const QString& gameName) { saveLM.setStringList(games[gameName]); });

  refresh();
  saveList.setModel(&saveLM);
  gameSelector.setModel(&gameLM);

  if (!OAuthNetAccess::SSLSupported()) {
    statusBar()->showMessage("No SSL library detected. Install " + oauthNetAccess.GetSSLBuildVersion());
    return;
  }

  statusBar()->showMessage("Requesting Google Discovery Doc from " + googleOAuth.GetDiscoveryDocEndpoint());
  QByteArray discoveryDoc{""};
  dialog.show();
  oauthNetAccess.Get(googleOAuth.GetDiscoveryDocEndpoint(), discoveryDoc);
  dialog.hide();
  QJsonObject discoveryDocObject{QJsonDocument::fromJson(discoveryDoc).object()};

  if (discoveryDocObject.isEmpty()) {
    statusBar()->showMessage("Endpoints could not be retrieved. Try again!");
    return;
  }

  googleOAuth.SetAuthEndpoint(
      discoveryDocObject[googleOAuth.GetAuthEndpointKeyName()]
          .toString()
          .toUtf8());
  googleOAuth.SetUserInfoEndpoint(
      discoveryDocObject[googleOAuth.GetUserInfoEndpointKeyName()]
          .toString()
          .toUtf8());

  statusBar()->showMessage("Endpoints retrieved. Log in when ready!");
  loginBtn.setEnabled(true);
}

void MainWindow::refresh(const bool&) {
  games = Game::BuildGames();
  gameLM.setStringList(games.keys());

  if (gameLM.stringList().length() > 0) gameSelector.setCurrentText(gameLM.stringList().first());
}

void MainWindow::login(const bool& unchecked) {
  googleOAuth.LogOut();
  if (!unchecked) {
    QFile::remove(profilePictureFilePath);
    loginBtn.setText(loginBtnDefaultValue);
    loginBtn.setIcon(QIcon{defaultProfilePictureFilePath});
    statusBar()->showMessage("Logged out");
    return;
  }

  loginBtn.setChecked(false);
  statusBar()->showMessage("Awaiting user consent...");

  if (!QDesktopServices::openUrl(QUrl{googleOAuth.BuildURL(OAuthLoopbackServer::GetListenPort())})) {
    statusBar()->showMessage("Could not open URL in desktop program!");
    return;
  }

  QByteArray consentResponse{""};
  dialog.show();
  switch (OAuthLoopbackServer::ListenForConsent(googleOAuth.GetRedirectUri(), loop, consentResponse)) {
    case ServerReturnCodes::SERVER_ERR:
      statusBar()->showMessage("An error occurred in the server!");
    case ServerReturnCodes::CANCELLED:
      statusBar()->showMessage("Request was cancelled!");
    default:
      dialog.hide();
  }

  switch (googleOAuth.HandleConsent(consentResponse)) {
    case OAuthReturnCodes::CONSENT_ERR:
      statusBar()->showMessage("An error occurred in handling the consent!");
      return;
    case OAuthReturnCodes::CONSENT_DENIED:
      statusBar()->showMessage("Consent was denied!");
      return;
    case OAuthReturnCodes::NON_UNIQUE_REQUEST:
      statusBar()->showMessage("Request was not unique!");
      return;
  }

  QByteArray userInfo{""};
  dialog.show();
  oauthNetAccess.Get(googleOAuth.GetUserInfoEndpoint(), googleOAuth.GetAccessToken(), userInfo);
  dialog.hide();
  if (userInfo == "") {
    statusBar()->showMessage("User info invalid!");
    return;
  }

  googleOAuth.SetUser(userInfo);
  loginBtn.setChecked(true);
  loginBtn.setText(googleOAuth.GetProfileName());
  statusBar()->showMessage("Logged in");

  QString profilePictureUrl{googleOAuth.GetProfilePictureURL()};
  if (profilePictureUrl == "") return;

  QByteArray profilePicture{""};
  dialog.show();
  oauthNetAccess.Get(profilePictureUrl, googleOAuth.GetAccessToken(), profilePicture);
  dialog.hide();
  if (profilePicture == "") {
    loginBtn.setIcon(QIcon{defaultProfilePictureFilePath});
    statusBar()->showMessage("Profile picture invalid!");
    return;
  }

  QFile profilePictureFile{profilePictureFilePath};
  if (!profilePictureFile.open(QFile::WriteOnly)) {
    statusBar()->showMessage("Could not open profile picture file!");
    return;
  }

  if (profilePictureFile.write(profilePicture) != -1) loginBtn.setIcon(QIcon{profilePictureFilePath});
}
} // namespace GS
