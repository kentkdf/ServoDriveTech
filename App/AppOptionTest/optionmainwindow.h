﻿#ifndef OPTIONMAINWINDOW_H
#define OPTIONMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class OptionMainWindow;
}

class OptionMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit OptionMainWindow(QWidget *parent = 0);
  ~OptionMainWindow();

private slots:
  void on_actionOption_triggered();

  void onToolBtnClicked();
  void onToolBtnActionClicked();
private:
  void closeEvent(QCloseEvent *e);

private:
  Ui::OptionMainWindow *ui;
};

#endif // OPTIONMAINWINDOW_H