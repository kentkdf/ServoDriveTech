﻿#include "uiplot.h"
#include "ui_uiplot.h"
#include "iuiwidget_p.h"

class UiPlotPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiPlot)
public:
  UiPlotPrivate();
  ~UiPlotPrivate();
protected:

};
UiPlotPrivate::UiPlotPrivate()
{

}
UiPlotPrivate::~UiPlotPrivate()
{

}


UiPlot::UiPlot(QWidget *parent):IUiWidget(*(new UiPlotPrivate),parent),ui(new Ui::UiPlot)
{
  ui->setupUi(this);

}
UiPlot::~UiPlot()
{
  delete ui;
}

QStackedWidget *UiPlot::getUiStackedWidget(void)
{
  return ui->stackedWidget;
}
QVBoxLayout *UiPlot::getVBoxLayout(void)
{
  return ui->verticalLayout_tree;
}
void UiPlot::setDefaultUi()
{
  setCurrentUiIndex(0);
}

QHBoxLayout *UiPlot::hBoxLayout()
{
  return ui->qmlHboxLayout;
}
