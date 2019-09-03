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
  QWidget gridLayoutWidget{};
  QGridLayout gridLayout{};
  QPushButton refreshBtn{};
  QComboBox gameSelector{};
  QListView saveList{};

  // GSOAuth gsOAuth{this->statusBar()};

  void refresh(const bool & = false);

public:
  explicit MainWindow();
};
} // namespace GS
