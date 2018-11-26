#pragma once
#include "gsoauth.h"
#include <QComboBox>
#include <QGridLayout>
#include <QListView>
#include <QMainWindow>
#include <QPushButton>
#include <QStringListModel>

namespace GS {
class MainWindow : public QMainWindow {
  QHash<QString, QStringList> games;

  QStringListModel gameLM{};
  QStringListModel saveLM{};
  QWidget centralWidget{this};
  QGridLayout gridLayout{&centralWidget};
  QComboBox gameSelector{};
  QPushButton refreshBtn{"Refresh"};
  QListView saveList{};

  void refresh(const bool & = false);

  GSOAuth gsOAuth{[=](QStatusBar *statusBar) { setStatusBar(statusBar); },
                  [=](QPushButton *loginBtn) {
                    gridLayout.addWidget(loginBtn, 2, 0, 1, 5);
                  }};

public:
  explicit MainWindow();
};
} // namespace GS
