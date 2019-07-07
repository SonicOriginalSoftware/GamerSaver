#include <QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QString customFileName = QCoreApplication::applicationDirPath() + "/styles/custom.css";
  QString styleFileName = QFile::exists(customFileName) ? customFileName : ":style.css";

  QFile styleFile{styleFileName};
  styleFile.open(QFile::ReadOnly);
  a.setStyleSheet(styleFile.readAll());
  styleFile.close();

  GS::MainWindow w{};
  w.show();

  return a.exec();
}
