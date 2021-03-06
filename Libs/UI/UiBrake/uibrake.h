﻿#ifndef UIBRAKE_H
#define UIBRAKE_H

#include "uibrake_global.h"
#include "iuiwidget.h"

namespace Ui {
class UiBrake;
}
class QWidget;
class UiBrakePrivate;

class UIBRAKESHARED_EXPORT UiBrake:public IUiWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(UiBrake)
public:
  explicit UiBrake(QWidget *parent = 0);
  ~UiBrake();

private:
  QStackedWidget *getUiStackedWidget(void)Q_DECL_OVERRIDE;
  QVBoxLayout *getVBoxLayout(void)Q_DECL_OVERRIDE;
  void setDefaultUi()Q_DECL_OVERRIDE;
  void setQmlContext()Q_DECL_OVERRIDE;
  void setQmlSignalSlot()Q_DECL_OVERRIDE;
  void addQmlWidget()Q_DECL_OVERRIDE;
private:
  Ui::UiBrake *ui;
};

#endif // UIBRAKE_H
