#pragma once
#include "gsoauth.h"
#include <QMainWindow>
#include <QStringListModel>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QListView>

namespace GS
{
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

  void refresh(const bool& = false);

public:
  explicit MainWindow();
};
} // namespace GS
