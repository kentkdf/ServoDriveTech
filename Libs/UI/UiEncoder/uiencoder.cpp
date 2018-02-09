﻿#include "uiencoder.h"
#include "ui_uiencoder.h"
#include "iuiwidget_p.h"
#include "igraphencoder.h"
#include "sevdevice.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <QDebug>


class UiEncoderPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiEncoder)
public:
  UiEncoderPrivate();
  ~UiEncoderPrivate();
protected:
  IGraphEncoder *m_graphEncoder;

};
UiEncoderPrivate::UiEncoderPrivate()
{

}
UiEncoderPrivate::~UiEncoderPrivate()
{

}

UiEncoder::UiEncoder(QWidget *parent):IUiWidget(*(new UiEncoderPrivate),parent),ui(new Ui::UiEncoder)
{
  ui->setupUi(this);

}
UiEncoder::~UiEncoder()
{
  Q_D(UiEncoder);
  delete ui;
  delete d->m_graphEncoder;
}

bool UiEncoder::hasConfigFunc()
{
  return false;
}

bool UiEncoder::hasSaveFunc()
{
  return true;
}
void UiEncoder::accept(QWidget *w)
{
  Q_D(UiEncoder);
  ui->qmlHboxLayout->addWidget(w);
  d->m_graphEncoder=dynamic_cast<IGraphEncoder *>(w);
  d->m_graphEncoder->visit(this);
}
void UiEncoder::setUiActive(bool actived)
{
  Q_D(UiEncoder);
  if(actived)
  {
    //读一次FLASH的值
    //开启编码器刷新定时器及错误检查
    if(readPageFLASH())
    {
      qDebug()<<"TEST_OUT readPageFLASH OK";
    }
  }
  else
  {
    //关闭编码器刷新定时器及错误检查
  }
}

QStackedWidget *UiEncoder::getUiStackedWidget(void)
{
  return ui->stackedWidget;
}

QVBoxLayout *UiEncoder::getVBoxLayout(void)
{
  return ui->verticalLayout_tree;
}
void UiEncoder::setDefaultUi()
{
  setCurrentUiIndex(0);
}
void UiEncoder::setQmlContext()
{

}

void UiEncoder::setQmlSignalSlot()
{

}

void UiEncoder::addQmlWidget()
{
  Q_D(UiEncoder);
  ui->qmlHboxLayout->addWidget(d->m_qwidget);
}
