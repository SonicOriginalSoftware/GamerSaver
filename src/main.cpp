#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QStyle>
#include <QtNetwork/QNetworkAccessManager>
#include <QTcpServer>
#include <QMessageBox>
#include "appmanifest.cpp"
#include "mainwindow.h"
#include "oauth.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName(GS::AppManifest::OrgName);
  QCoreApplication::setApplicationName(GS::AppManifest::AppName);
  QCoreApplication::setApplicationVersion(GS::AppManifest::Version);

  a.setStyleSheet(
    "* {"
      "font-size: 18px;"
    "}"
    ""
    "#loginBtn {"
      "qproperty-iconSize: 48px;"
      "font-size: 24px;"
    "}"
  );

  QFile styleFile{QCoreApplication::applicationDirPath() + "/styles/custom.css"};
  if (styleFile.open(QFile::ReadOnly)) a.setStyleSheet(styleFile.readAll());

  QMessageBox dialog{QMessageBox::Icon::NoIcon,
                         "GamerSaver - Waiting...",
                         "Press Cancel to abort the request",
                         QMessageBox::Cancel};
  QNetworkAccessManager qnam{};
  QTcpServer loopbackServer{};
  QObject::connect(&qnam, &QNetworkAccessManager::finished, &dialog, &QMessageBox::accept);
  QObject::connect(&loopbackServer, &QTcpServer::newConnection, &dialog, &QMessageBox::accept);

  GS::OAuth2 oauth{qnam, loopbackServer};
  GS::MainWindow w{oauth};

  w.setFixedSize(1000, 500);
  w.move((qApp->desktop()->availableGeometry().width() / 2)
         - (w.width() / 2),
         (qApp->desktop()->availableGeometry().height() / 2)
         - (w.height() / 2));
  w.show();

  return a.exec();
}
