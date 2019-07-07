#include "appmanifest.cpp"
#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName(GS::AppManifest::OrgName);
  QCoreApplication::setApplicationName(GS::AppManifest::AppName);
  QCoreApplication::setApplicationVersion(GS::AppManifest::Version);

  QString customFileName = QCoreApplication::applicationDirPath() + "/styles/custom.css";
  QString styleFileName = QFile::exists(customFileName) ? customFileName : ":/res/style.css";

  QFile styleFile{styleFileName};
  if (styleFile.open(QFile::ReadOnly))
  {
    a.setStyleSheet(styleFile.readAll());
    styleFile.close();
  }

  GS::MainWindow w{};
  w.show();

  return a.exec();
}

