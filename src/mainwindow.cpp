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
#include <QMessageBox>
#include <QUrl>
#include <QtWidgets/QStatusBar>

namespace GS {
const QString MainWindow::loginBtnDefaultValue{"Login"};
const QString MainWindow::defaultProfilePictureFilePath{":/res/login_placeholder.svg"};

MainWindow::MainWindow(GoogleOAuth& oauth, OAuthNetAccess& net, QMessageBox& dialog)
    : profilePictureFilePath{QCoreApplication::applicationDirPath().toUtf8() + "profilePic.jpg"},
      dialog{dialog}, googleOAuth{oauth}, oauthNetAccess{net}
{
  refresh();
  refreshBtn.setText("Refresh");

  loginBtn.setCheckable(true);
  loginBtn.setText(loginBtnDefaultValue);
  loginBtn.setIcon(QIcon(defaultProfilePictureFilePath));

  setWindowTitle("GamerSaver");
  setObjectName("MainWindow");
  gridLayoutWidget.setObjectName("gridLayoutWidget");
  gridLayout.setObjectName("gridLayout");
  loginBtn.setObjectName("loginBtn");
  refreshBtn.setObjectName("refreshBtn");
  gameSelector.setObjectName("gameSelector");
  saveList.setObjectName("saveList");

  setCentralWidget(&gridLayoutWidget);
  gridLayout.setContentsMargins(4, 4, 4, 4);
  gridLayout.addWidget(&gameSelector, 0, 0, 1, 4);
  gridLayout.addWidget(&refreshBtn, 0, 4, 1, 1);
  gridLayout.addWidget(&saveList, 1, 0, 1, 5);
  gridLayout.addWidget(&loginBtn, 2, 0, 1, 5);
  QMetaObject::connectSlotsByName(this);

  saveList.setModel(&saveLM);
  gameSelector.setModel(&gameLM);
}

void MainWindow::refresh() {
  games = Game::BuildGames();
  gameLM.setStringList(games.keys());
  if (gameLM.stringList().length() > 0) gameSelector.setCurrentText(gameLM.stringList().first());

  if (OAuthNetAccess::SSLSupported() && oauthNetAccess.NetworkConnected()) {
    statusBar()->showMessage("Requesting Google Discovery Doc from " + googleOAuth.GetDiscoveryDocEndpoint());

    QJsonObject discoveryDocObject{
        QJsonDocument::fromJson(
            oauthNetAccess.Get(googleOAuth.GetDiscoveryDocEndpoint(), dialog))
            .object()};

    googleOAuth.SetAuthEndpoint(
        discoveryDocObject[googleOAuth.GetAuthEndpointKeyName()]
            .toString()
            .toUtf8());
    googleOAuth.SetUserInfoEndpoint(
        discoveryDocObject[googleOAuth.GetUserInfoEndpointKeyName()]
            .toString()
            .toUtf8());
  } else {
    statusBar()->showMessage("No SSL library detected. Install " + oauthNetAccess.GetSSLBuildVersion());
    loginBtn.setEnabled(false);
  }
}

void MainWindow::on_gameSelector_currentTextChanged(const QString& gameName) {
  saveLM.setStringList(games[gameName]);
}

void MainWindow::on_refreshBtn_clicked(const bool&) { refresh(); }

void MainWindow::on_loginBtn_clicked(const bool& unchecked) {
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

  if (!QDesktopServices::openUrl(
          QUrl{googleOAuth.BuildURL(OAuthLoopbackServer::GetListenPort())})) {
    statusBar()->showMessage("Could not open URL in desktop program!");
    return;
  }

  switch (googleOAuth.HandleConsent(OAuthLoopbackServer::PromptForConsent(
      googleOAuth.GetRedirectUri(), dialog))) {
    case ReturnCodes::CONSENT_ERR:
      statusBar()->showMessage("An error occurred in handling the consent");
      return;
    case ReturnCodes::CONSENT_DENIED:
      statusBar()->showMessage("Consent was denied");
      return;
    case ReturnCodes::NON_UNIQUE_REQUEST:
      statusBar()->showMessage("Request was not unique!");
      return;
    default:
      break;
  }

  googleOAuth.SetUser(oauthNetAccess.Get(googleOAuth.GetUserInfoEndpoint(), dialog, googleOAuth.GetAccessToken()));

  loginBtn.setChecked(true);
  loginBtn.setText(googleOAuth.GetProfileName());
  statusBar()->showMessage("Logged in");

  QByteArray profilePicture{oauthNetAccess.Get(googleOAuth.GetProfilePictureURL(), dialog, googleOAuth.GetAccessToken())};

  QFile profilePictureFile{profilePictureFilePath};
  if (!profilePictureFile.open(QFile::WriteOnly)) {
    statusBar()->showMessage("Could not open profile picture file!");
    return;
  }

  if (profilePictureFile.write(profilePicture) != -1) loginBtn.setIcon(QIcon{profilePictureFilePath});
}
} // namespace GS

