#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QFile>

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName(GS::AppManifest::OrgName);
  QCoreApplication::setApplicationName(GS::AppManifest::AppName);
  QCoreApplication::setApplicationVersion(GS::AppManifest::Version);

  QApplication a(argc, argv);
  a.setStyleSheet("*{font-size:18px;} #loginBtn{qproperty-iconSize:48px;font-size:24px;}");

  QFile styleFile{QCoreApplication::applicationDirPath() + "/styles/custom.css"};
  if (styleFile.open(QFile::ReadOnly)) a.setStyleSheet(styleFile.readAll());

  GS::MainWindow w{};

  w.setFixedSize(1000, 500);
  QScreen *screen{QGuiApplication::primaryScreen()}; // Should this be deleted!?
  w.move((screen->geometry().width() / 2) - (w.width() / 2),
         (screen->geometry().height() / 2) - (w.height() / 2));
  w.show();

  return a.exec();
}

