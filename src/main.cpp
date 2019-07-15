#include <QApplication>
#include <QFile>
#include <QDebug>
#include "appmanifest.cpp"
#include "mainwindow.h"

void tryToSetStylesheet(QApplication& a)
{
  QString customFileName = QCoreApplication::applicationDirPath() + "/styles/custom.css";
  QString styleFileName = QFile::exists(customFileName) ? customFileName : ":/res/style.css";

  QFile styleFile{styleFileName};
  qDebug() << "Setting stylesheet to that of" << styleFileName;
  if (styleFile.open(QFile::ReadOnly)) a.setStyleSheet(styleFile.readAll());
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName(GS::AppManifest::OrgName);
  QCoreApplication::setApplicationName(GS::AppManifest::AppName);
  QCoreApplication::setApplicationVersion(GS::AppManifest::Version);

  tryToSetStylesheet(a);

  GS::MainWindow w{};
  w.show();

  return a.exec();
}

