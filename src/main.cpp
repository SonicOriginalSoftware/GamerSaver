#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include "appmanifest.cpp"
#include "mainwindow.h"

void tryToSetStylesheet(QApplication& a)
{
  QString customFileName = QCoreApplication::applicationDirPath() + "/styles/custom.css";
  QString styleFileName = QFile::exists(customFileName) ? customFileName : ":/res/style.css";

  QFile styleFile{styleFileName};
  if (styleFile.open(QFile::ReadOnly)) a.setStyleSheet(styleFile.readAll());
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName(GS::AppManifest::OrgName);
  QCoreApplication::setApplicationName(GS::AppManifest::AppName);
  QCoreApplication::setApplicationVersion(GS::AppManifest::Version);

  tryToSetStylesheet(a);

  GS::MainWindow w{};
  w.ensurePolished();
  int windowWidth{w.size().width()};
  int windowHeight{w.size().height()};

  w.setGeometry(((a.desktop()->screen()->width() - windowWidth) / 2),
                ((a.desktop()->screen()->height() - windowHeight) / 2),
                windowWidth,
                windowHeight
               );
  w.show();

  return a.exec();
}
