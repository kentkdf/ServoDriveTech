﻿#ifndef UIFLASH_H
#define UIFLASH_H

#include "uiflash_global.h"
#include "iuiwidget.h"

namespace Ui {
class UiFLASH;
}
class QWidget;
class UiFLASHPrivate;

class UiFLASH:public IUiWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(UiFLASH)
public:
  explicit UiFLASH(QWidget *parent = 0);
  ~UiFLASH();

  QStackedWidget *getUiStackedWidget(void)Q_DECL_OVERRIDE;
  QVBoxLayout *getVBoxLayout(void)Q_DECL_OVERRIDE;
  void setDefaultUi()Q_DECL_OVERRIDE;
  void readPageFLASH()Q_DECL_OVERRIDE{}
  void writePageFLASH()Q_DECL_OVERRIDE{}

private:
  Ui::UiFLASH *ui;
};

#endif // UIFLASH_H
