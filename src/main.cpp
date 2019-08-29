#include "appmanifest.h"
#include "googleoauth.h"
#include "mainwindow.h"
#include "oauthloopbackserver.h"
#include "oauthnetaccess.h"
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QNetworkReply>
#include <QScreen>
#include <QtNetwork/QNetworkAccessManager>

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName(GS::AppManifest::OrgName);
  QCoreApplication::setApplicationName(GS::AppManifest::AppName);
  QCoreApplication::setApplicationVersion(GS::AppManifest::Version);

  QMessageBox dialog{QMessageBox::Icon::NoIcon, "GamerSaver - Waiting...",
                     "Press Cancel to abort the request", QMessageBox::Cancel};

  QNetworkAccessManager qnam{};
  QObject::connect(&qnam, &QNetworkAccessManager::finished, &dialog,
                   &QMessageBox::accept);

  GS::OAuthNetAccess oauthNetAccess{qnam};
  GS::GoogleOAuth googleOauth(GS::AppManifest::ClientID, GS::AppManifest::RedirectURI);
  GS::MainWindow w{googleOauth, oauthNetAccess, dialog};

  w.setFixedSize(1000, 500);
  QScreen *screen{QGuiApplication::primaryScreen()};
  w.move((screen->geometry().width() / 2) - (w.width() / 2),
         (screen->geometry().height() / 2) - (w.height() / 2));
  w.show();

  QApplication a(argc, argv);
  a.setStyleSheet("*{font-size:18px;} #loginBtn{qproperty-iconSize:48px;font-size:24px;}");

  QFile styleFile{QCoreApplication::applicationDirPath() + "/styles/custom.css"};
  if (styleFile.open(QFile::ReadOnly)) a.setStyleSheet(styleFile.readAll());

  return a.exec();
}

