#include <QHash>
#include <QStringListModel>
#include <QtGui/QIcon>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "game.h"
#include "oauth.h"
#include "mainwindow.h"

GS::MainWindow::MainWindow(OAuth2& oauth) : games{GS::Game::BuildGames()}, _oauth{oauth}
{
  if (!oauth.SSLSupported())
  {
    statusBar()->showMessage("No SSL library detected.");
    loginBtn.setEnabled(false);
  }

  gameLM.setStringList(games.keys());
  refreshBtn.setText("Refresh");
  loginBtn.setIcon(QIcon(OAuth2::DefaultPictureURL()));

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

  loginBtn.setCheckable(true);

  saveList.setModel(&saveLM);
  gameSelector.setModel(&gameLM);
}

void GS::MainWindow::on_gameSelector_currentTextChanged(const QString& gameName)
{
  saveLM.setStringList(games[gameName]);
}

void GS::MainWindow::on_refreshBtn_clicked(const bool&)
{
  games = GS::Game::BuildGames();
  gameLM.setStringList(games.keys());
  if (gameLM.stringList().length() > 0) gameSelector.setCurrentText(gameLM.stringList().first());
}

void GS::MainWindow::on_loginBtn_clicked(const bool& unchecked)
{
  if (!unchecked)
  {
    _oauth.LogOut();
    statusBar()->showMessage("Logged out");

    loginBtn.setText(_oauth.ProfileName());
    loginBtn.setIcon(QIcon{_oauth.ProfilePictureURL()});
    return;
  }

  loginBtn.setChecked(false);
  _oauth.LogOut();
  statusBar()->showMessage("Requesting Google Discovery Doc from " +
                            OAuth2::OAuthEndpoints::GoogleDiscoveryDoc);
  switch (_oauth.PopulateGoogleEndpoints())
  {
    case ReturnCodes::CANCELLED:
      statusBar()->showMessage("Discovery request cancelled!");
      return;
    case GS::ReturnCodes::NETWORK_ERR:
      statusBar()->showMessage("Discovery endpoint could not be reached!");
      return;
  }

  statusBar()->showMessage("Awaiting user consent...");
  switch (_oauth.PromptForConsent())
  {
    case ReturnCodes::UNHANDLED:
      statusBar()->showMessage("Could not handle opening URL in default system browser!");
      return;
    case ReturnCodes::CANCELLED:
      statusBar()->showMessage("Request for consent was cancelled!");
      return;
    case ReturnCodes::SERVER_ERR:
      statusBar()->showMessage("The localhost server could not be started!");
      return;
  }

  switch (_oauth.HandleConsent())
  {
    case ReturnCodes::CONSENT_ERR:
      statusBar()->showMessage("An error occurred in handling the consent");
      return;
    case ReturnCodes::CONSENT_DENIED:
      statusBar()->showMessage("Consent was denied");
      return;
    case ReturnCodes::NON_UNIQUE_REQUEST:
      statusBar()->showMessage("Request was not unique!");
      return;
  }

  switch (_oauth.SetUser())
  {
    case ReturnCodes::NETWORK_ERR:
      statusBar()->showMessage("Could not retrieve user info!");
      return;
    case ReturnCodes::CANCELLED:
      statusBar()->showMessage("User info request cancelled!");
      return;
  }

  loginBtn.setChecked(true);
  statusBar()->showMessage("Logged in");

  switch (_oauth.UpdateProfilePicture())
  {
    case ReturnCodes::OK:
      break;
    case ReturnCodes::IO_ERR:
      statusBar()->showMessage("Could not open profile picture file!");
      return;
    case ReturnCodes::CANCELLED:
      statusBar()->showMessage("Profile picture request cancelled!");
      return;
    default:
      statusBar()->showMessage("Could not retrieve profile picture!");
      return;
  }

  loginBtn.setText(_oauth.ProfileName());
  loginBtn.setIcon(QIcon{_oauth.ProfilePictureURL()});
}
