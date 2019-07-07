#pragma once
#include <QMainWindow>

// Forward declarations
class QListView;
class QGridLayout;
class QPushButton;
class QComboBox;
class QStringListModel;
class QNetworkAccessManager;

namespace GS {
class OAuth2;

class MainWindow : public QMainWindow {
  Q_OBJECT

  QNetworkAccessManager *nam{nullptr};

  QStringListModel *gameLM{nullptr};
  QStringListModel *saveLM{nullptr};
  QWidget *gridLayoutWidget{nullptr};
  QGridLayout *gridLayout{nullptr};
  QPushButton *loginBtn{nullptr};
  QComboBox *gameSelector{nullptr};
  QPushButton *refreshBtn{nullptr};
  QListView *saveList{nullptr};

  OAuth2 *oauth{nullptr};

private slots:
  void on_gameSelector_currentTextChanged(const QString &) const;
  void on_refreshBtn_clicked(const bool &) const;
  void on_loginBtn_clicked(const bool &) const;

public:
  explicit MainWindow();
  ~MainWindow();
};
} // namespace GS
