#pragma once
#include "gsoauth.h"
#include <QMainWindow>
#include <QStringListModel>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QListView>

namespace GS {
class MainWindow : public QMainWindow {
  QHash<QString, QStringList> games;

  QStringListModel gameLM{};
  QStringListModel saveLM{};
  QGridLayout gridLayout{};
  QWidget gridLayoutWidget{};
  QComboBox gameSelector{};
  QPushButton refreshBtn{};
  QListView saveList{};

  void refresh(const bool & = false);

  // GSOAuth gsOAuth{*this, [=](QPushButton* loginBtn)
  //                        {gridLayout.addWidget(loginBtn, 2, 0, 1, 5);}};

public:
  explicit MainWindow();
};
} // namespace GS
