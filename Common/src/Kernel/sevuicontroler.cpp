﻿#include "sevuicontroler.h"
#include "sevdevice.h"
#include "sdtglobaldef.h"
#include "uifactory.h"
#include "iuiwidget.h"

#include <QDebug>
#include <QTreeWidgetItem>

SevUiControler::SevUiControler(SevDevice *sev, OptContainer *gconfig, QObject *parent):IUiControler(gconfig,parent),
  m_sev(sev)
{

}
void SevUiControler::createUis()
{
  QTreeWidgetItem *targetTree=m_sev->targetTree();
  Q_ASSERT(targetTree);

  QTreeWidgetItem *axisItem=targetTree->child(0);
  QTreeWidgetItem *globalItem=targetTree->child(1);

  IUiWidget *ui=NULL;
  QString className;
  double sum=65;
  double dec=35.0/(m_sev->axisNum()*axisItem->childCount());
  //与轴相关的模块

  for(int i=0;i<m_sev->axisNum();i++)
  {
    for(int j=0;j<axisItem->childCount();j++)
    {
      className=axisItem->child(j)->text(2);

      ui=dynamic_cast<IUiWidget *> (UiFactory::createObject(className.toLatin1()));
      ui->init(m_sev);
      UiIndexs index;
      index.devInx=m_sev->devId();
      index.aixsInx=i;
      index.pageInx=j;
      ui->setUiIndexs(index);
      ui->addTreeWidget(m_sev->axisTreeSource(i,j));
      ui->createQmlWidget();
//      qDebug()<<"ui->objectName()"<<ui->objectName();
//      connect(ui,SIGNAL(sglReadPageFlash(int,QTreeWidget*)),m_sev,SLOT(onReadPageFlash(int,QTreeWidget*)));
//      connect(ui,SIGNAL(sglWritePageFlash(int,QTreeWidget*)),m_sev,SLOT(onWritePageFlash(int,QTreeWidget*)));

      m_uiLists.append(ui);
//      qDebug()<<"class name "<<ui->objectName();
      sum+=dec;
//      qDebug()<<"sum"<<sum;
      emit initProgressInfo((int)sum,tr("build ui %1").arg(className));
    }
    qDebug()<<"------------------build device ui :"<<i;
  }
  //单个设备中通用模块
  for(int i=0;i<globalItem->childCount();i++)
  {
    className=globalItem->child(i)->text(2);
    ui=dynamic_cast<IUiWidget *> (UiFactory::createObject(className.toLatin1()));
    ui->init(m_sev);
    UiIndexs index;
    index.devInx=m_sev->devId();
    index.aixsInx=-1;
    index.pageInx=i;
    ui->setUiIndexs(index);
    ui->addTreeWidget(m_sev->globalTreeSource(i));
    connect(ui,SIGNAL(sglReadPageFlash(int,QTreeWidget*)),m_sev,SLOT(onReadPageFlash(int,QTreeWidget*)));

    m_uiLists.append(ui);
//    qDebug()<<"class name "<<ui->objectName();
    sum=100;
//    qDebug()<<"sum"<<sum;
    emit initProgressInfo((int)sum,tr("build ui %1").arg(className));
  }
}

SevUiControler::~SevUiControler()
{
  qDebug()<<"SevUiControler-->destruct";
  GT::deepClearList(m_uiLists);
}

IUiWidget *SevUiControler::uiWidget(quint32 devInx,qint16 axisInx,const QString &uiName)const
{
  IUiWidget *ui=NULL;
  foreach (IUiWidget *w, m_uiLists)
  {
    UiIndexs indexs=w->uiIndexs();
    if(indexs.devInx==devInx&&indexs.aixsInx==axisInx&&w->objectName()==uiName)
    {
      ui=w;
      break;
    }
  }
  return ui;
}
