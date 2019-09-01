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

MainWindow::MainWindow() : profilePictureFilePath{QCoreApplication::applicationDirPath().toUtf8() + "profilePic.jpg"}
{
  dialog.setIcon(QMessageBox::Icon::NoIcon);
  dialog.setWindowTitle("GamerSaver - Waiting...");
  dialog.setText("Press Cancel to abort the request");
  dialog.setStandardButtons(QMessageBox::Cancel);

  QObject::connect(&dialog, &QMessageBox::finished, &loop, &QEventLoop::exit);

  refresh();
  refreshBtn.setText("Refresh");

  loginBtn.setCheckable(true);
  loginBtn.setText(loginBtnDefaultValue);
  loginBtn.setIcon(QIcon(defaultProfilePictureFilePath));
  loginBtn.setObjectName("loginBtn");

  setWindowTitle("GamerSaver");
  setCentralWidget(&gridLayoutWidget);
  gridLayoutWidget.setLayout(&gridLayout);

  gridLayout.setContentsMargins(4, 4, 4, 4);
  gridLayout.addWidget(&gameSelector, 0, 0, 1, 4);
  gridLayout.addWidget(&refreshBtn, 0, 4, 1, 1);
  gridLayout.addWidget(&saveList, 1, 0, 1, 5);
  gridLayout.addWidget(&loginBtn, 2, 0, 1, 5);

  // QObject::connect(&refreshBtn, &QPushButton::clicked, this, &MainWindow::refresh);
  // QObject::connect(&loginBtn, &QPushButton::clicked, this, &MainWindow::login);
  QObject::connect(&gameSelector, &QComboBox::currentTextChanged,
      [=](const QString& gameName) { saveLM.setStringList(games[gameName]); });

  saveList.setModel(&saveLM);
  gameSelector.setModel(&gameLM);
}

void MainWindow::refresh(const bool&) {
  games = Game::BuildGames();
  gameLM.setStringList(games.keys());
  loginBtn.setEnabled(false);

  if (gameLM.stringList().length() > 0) gameSelector.setCurrentText(gameLM.stringList().first());

  if (!OAuthNetAccess::SSLSupported()) {
    statusBar()->showMessage("No SSL library detected. Install " + oauthNetAccess.GetSSLBuildVersion());
    return;
  }
  else if (!oauthNetAccess.NetworkConnected())
  {
    statusBar()->showMessage("No network connectivity detected. Connect to the internet and try again!");
    return;
  }

  statusBar()->showMessage("Requesting Google Discovery Doc from " + googleOAuth.GetDiscoveryDocEndpoint());

  dialog.show();
  QJsonObject discoveryDocObject{
      QJsonDocument::fromJson(
          oauthNetAccess.Get(googleOAuth.GetDiscoveryDocEndpoint()))
          .object()};

  googleOAuth.SetAuthEndpoint(
      discoveryDocObject[googleOAuth.GetAuthEndpointKeyName()]
          .toString()
          .toUtf8());
  googleOAuth.SetUserInfoEndpoint(
      discoveryDocObject[googleOAuth.GetUserInfoEndpointKeyName()]
          .toString()
          .toUtf8());
  loginBtn.setEnabled(true);
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

  // TODO Show the dialog
  switch (googleOAuth.HandleConsent(OAuthLoopbackServer::PromptForConsent(googleOAuth.GetRedirectUri(), loop))) {
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

  // TODO Show the dialog
  googleOAuth.SetUser(oauthNetAccess.Get(googleOAuth.GetUserInfoEndpoint(), googleOAuth.GetAccessToken()));

  loginBtn.setChecked(true);
  loginBtn.setText(googleOAuth.GetProfileName());
  statusBar()->showMessage("Logged in");

  // TODO Show the dialog
  QByteArray profilePicture{oauthNetAccess.Get(googleOAuth.GetProfilePictureURL(), googleOAuth.GetAccessToken())};

  QFile profilePictureFile{profilePictureFilePath};
  if (!profilePictureFile.open(QFile::WriteOnly)) {
    statusBar()->showMessage("Could not open profile picture file!");
    return;
  }

  if (profilePictureFile.write(profilePicture) != -1) loginBtn.setIcon(QIcon{profilePictureFilePath});
}
} // namespace GS
