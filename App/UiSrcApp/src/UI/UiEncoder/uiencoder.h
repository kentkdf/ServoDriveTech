﻿#ifndef UIENCODER_H
#define UIENCODER_H

#include "uiencoder_global.h"
#include "iuiwidget.h"

namespace Ui {
class UiEncoder;
}
class QWidget;
class UiEncoderPrivate;
class UiEncoder:public IUiWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(UiEncoder)
public:
  explicit UiEncoder(QWidget *parent = 0);
  ~UiEncoder();

  QStackedWidget *getUiStackedWidget(void)Q_DECL_OVERRIDE;
  QVBoxLayout *getVBoxLayout(void)Q_DECL_OVERRIDE;
  void setDefaultUi()Q_DECL_OVERRIDE;

private:
  Ui::UiEncoder *ui;
};

#endif // UIENCODER_H
