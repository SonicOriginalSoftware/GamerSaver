#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
  QCoreApplication::setOrganizationName(GS::AppManifest::OrgName);
  QCoreApplication::setApplicationName(GS::AppManifest::AppName);
  QCoreApplication::setApplicationVersion(GS::AppManifest::Version);

  QApplication a(argc, argv);
  a.setStyleSheet(
    "*{font-size:18px;}"
    "#loginBtn{qproperty-iconSize:48px;font-size:24px;}"
  );

  QFile styleFile{QCoreApplication::applicationDirPath() + "/custom.css"};
  if (styleFile.open(QFile::ReadOnly)) a.setStyleSheet(styleFile.readAll());

  GS::MainWindow w{};

  w.setFixedSize(1000, 500);
  w.show();

  return a.exec();
}
