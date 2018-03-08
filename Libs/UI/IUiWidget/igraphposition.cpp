﻿#include "igraphposition.h"
#include "igraphposition_p.h"
#include "boxitemmapping.h"
#include "SDTGraphicsItems"
#include "Option"
#include "gtutils.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QTreeWidgetItem>

#define PID_POS_X -220
#define PID_POS_Y 0

IGraphPositionPrivate::IGraphPositionPrivate()
{

}

IGraphPositionPrivate::~IGraphPositionPrivate()
{

}


IGraphPosition::~IGraphPosition()
{

}

void IGraphPosition::syncTreeDataToUiFace()
{
  Q_D(IGraphPosition);
  d->m_mapping->syncAllItem2BoxText();
}

void IGraphPosition::createItems()
{
  createInputFilterItem();
  createSumItem0();
  createPidItem();
  createSaturationItem();
  createSumItem1();
  createVelocityControllerItem();
  createSumItem2();
  createCurrentControllerItem();
  createPositionFeedbackItem();
  createFFVelocityItem();
  createFFAccelerationItem();
  createTargetItems();

  createStartEndTargetItems();
  createStartTextItem();
  createEndTextItem();

  createArrowItems();
  createAnchorItemHelper();
  setUpItemPosAnchors();

  adjustPosition();
  setCustumBackgroundColor();
  setBackGroundColor(QColor(Qt::white));
}

void IGraphPosition::setDoubleSpinBoxConnections()
{
  Q_D(IGraphPosition);
  connect(d->m_pEdit,SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
  connect(d->m_accEdit,SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
  connect(d->m_velEdit,SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
}

void IGraphPosition::installDoubleSpinBoxEventFilter()
{
  Q_D(IGraphPosition);
  d->m_pEdit->installEventFilter(this);
  d->m_accEdit->installEventFilter(this);
  d->m_velEdit->installEventFilter(this);
}

void IGraphPosition::adjustPosition()
{
  Q_D(IGraphPosition);
  if(d->m_anchorHelper!=NULL)
    d->m_anchorHelper->setAnchorsActive();

  GTUtils::delayms(10);
  foreach (ArrowItem *arrow, d->m_arrowList) {
    arrow->updatePosition();
    arrow->setColor(arrowColor());
  }
}

void IGraphPosition::createInputFilterItem()
{
  Q_D(IGraphPosition);
  LabelItemWidget *label=new LabelItemWidget(tr("InputFilter"));
  label->setObjectName("label_posInputFilter");
  label->setAlignment(Qt::AlignCenter);
  d->m_UIF=new WidgetItem;
  d->m_UIF->setWidget(label,true);
  d->m_scene->addItem(d->m_UIF);
}

void IGraphPosition::createSumItem0()
{
  Q_D(IGraphPosition);
  SumItemWidget *sw=new SumItemWidget;
  sw->setObjectName("sumItemWidget_posSum0");
  d->m_USUM0=new WidgetItem;
  d->m_USUM0->setWidget(sw);
  d->m_scene->addItem(d->m_USUM0);
}

void IGraphPosition::createPidItem()
{
  Q_D(IGraphPosition);

  QWidget *wpid=new QWidget;
  wpid->setObjectName("widget_posPid");
  QVBoxLayout *vlayoutTest=new QVBoxLayout(wpid);
  QLabel *title=new QLabel(tr("PID controller"),wpid);
  title->setObjectName("label_posTitle");
  title->setAlignment(Qt::AlignCenter);
  vlayoutTest->addWidget(title);

  QLabel *pgain=new QLabel(tr("P gain(HZ)"));
  pgain->setObjectName("label_posPgain");
  vlayoutTest->addWidget(pgain);
  QDoubleSpinBox *pedit=new QDoubleSpinBox(wpid);
  d->m_pEdit=pedit;
  pedit->setObjectName("dspinBox_posPedit");
  pedit->setMinimum(-32768);
  pedit->setMaximum(32767);
  pedit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  vlayoutTest->addWidget(pedit);

  wpid->setLayout(vlayoutTest);

  d->m_UPID=new WidgetItem;
  d->m_UPID->setWidget(wpid,true);
  d->m_scene->addItem(d->m_UPID);//take ownership
}

void IGraphPosition::createSaturationItem()
{
  Q_D(IGraphPosition);
  SaturationItemWidget *saturation=new SaturationItemWidget;
  saturation->setObjectName("saturationItemWidget_posSaturation");
  d->m_USATN=new WidgetItem;
  d->m_USATN->setWidget(saturation);
  d->m_scene->addItem(d->m_USATN);
}

void IGraphPosition::createSumItem1()
{
  Q_D(IGraphPosition);
  SumItemWidget *sw=new SumItemWidget;
  sw->setObjectName("sumItemWidget_posSum1");
  d->m_USUM1=new WidgetItem;
  d->m_USUM1->setWidget(sw);
  d->m_scene->addItem(d->m_USUM1);
}

void IGraphPosition::createVelocityControllerItem()
{
  Q_D(IGraphPosition);
  LabelItemWidget *label=new LabelItemWidget(tr("VelCtler"));
  label->setObjectName("label_posVelCtler");
  label->setAlignment(Qt::AlignCenter);
  d->m_UVCTL=new WidgetItem;
  d->m_UVCTL->setWidget(label,true);
  d->m_scene->addItem(d->m_UVCTL);
}

void IGraphPosition::createSumItem2()
{
  Q_D(IGraphPosition);
  SumItemWidget *sw=new SumItemWidget;
  sw->setObjectName("sumItemWidget_posSum2");
  d->m_USUM2=new WidgetItem;
  d->m_USUM2->setWidget(sw);
  d->m_scene->addItem(d->m_USUM2);
}

void IGraphPosition::createCurrentControllerItem()
{
  Q_D(IGraphPosition);
  LabelItemWidget *label=new LabelItemWidget(tr("CurCtler"));
  label->setObjectName("label_posCurCtler");
  label->setAlignment(Qt::AlignCenter);
  d->m_UCCTL=new WidgetItem;
  d->m_UCCTL->setWidget(label,true);
  d->m_scene->addItem(d->m_UCCTL);
}

void IGraphPosition::createPositionFeedbackItem()
{
  Q_D(IGraphPosition);
  LabelItemWidget *label=new LabelItemWidget(tr("PosFeedback"));
  label->setObjectName("label_posPosFeedback");
  label->setAlignment(Qt::AlignCenter);
  d->m_UPB=new WidgetItem;
  d->m_UPB->setWidget(label,true);
  d->m_scene->addItem(d->m_UPB);
}

void IGraphPosition::createFFVelocityItem()
{
  Q_D(IGraphPosition);

  QWidget *w=new QWidget;
  w->setObjectName("widget_posFFvel");
  QVBoxLayout *vlayoutTest=new QVBoxLayout(w);

  QLabel *label=new QLabel(tr("velocity feedforward (%)"));
  label->setObjectName("label_posFFvel");
  vlayoutTest->addWidget(label);
  QDoubleSpinBox *edit=new QDoubleSpinBox(w);
  d->m_velEdit=edit;
  edit->setObjectName("dspinBox_posFFvel");
  edit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  vlayoutTest->addWidget(edit);

  w->setLayout(vlayoutTest);

  d->m_UFVB=new WidgetItem;
  d->m_UFVB->setWidget(w,true);
  d->m_scene->addItem(d->m_UFVB);//take ownership
}

void IGraphPosition::createFFAccelerationItem()
{
  Q_D(IGraphPosition);

  QWidget *w=new QWidget;
  w->setObjectName("widget_posFFacc");
  QVBoxLayout *vlayoutTest=new QVBoxLayout(w);

  QLabel *label=new QLabel(tr("acceleration feedforward (%)"));
  label->setObjectName("label_posFFacc");
  vlayoutTest->addWidget(label);
  QDoubleSpinBox *edit=new QDoubleSpinBox(w);
  d->m_accEdit=edit;
  edit->setObjectName("dspinBox_posFFacc");
  edit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  vlayoutTest->addWidget(edit);

  w->setLayout(vlayoutTest);

  d->m_UFAB=new WidgetItem;
  d->m_UFAB->setWidget(w,true);
  d->m_scene->addItem(d->m_UFAB);//take ownership
}

void IGraphPosition::createTargetItems()
{
  Q_D(IGraphPosition);
  TargetItemWidget *t0=new TargetItemWidget;
  t0->setObjectName("targetItem_posTm0");
  d->m_TM0=new WidgetItem;
  d->m_TM0->setWidget(t0);

  t0=new TargetItemWidget;
  t0->setObjectName("targetItem_posT0");
  d->m_T0=new WidgetItem;
  d->m_T0->setWidget(t0);

  d->m_scene->addItem(d->m_TM0);
  d->m_scene->addItem(d->m_T0);
}

void IGraphPosition::createArrowItems()
{
  Q_D(IGraphPosition);
  d->m_A0=new ArrowItem(d->m_Tstart->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UIF->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A1=new ArrowItem(d->m_UIF->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_TM0->pointF(WidgetItem::POINT_TYPE_LEFT),ArrowItem::ARROW_TYPE_STRAIGHT,"",false);
  d->m_A2=new ArrowItem(d->m_TM0->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USUM0->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A3=new ArrowItem(d->m_USUM0->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UPID->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A4=new ArrowItem(d->m_UPID->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USATN->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A5=new ArrowItem(d->m_USATN->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USUM1->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A6=new ArrowItem(d->m_USUM1->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UVCTL->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A7=new ArrowItem(d->m_UVCTL->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USUM2->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A8=new ArrowItem(d->m_USUM2->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UCCTL->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A9=new ArrowItem(d->m_UCCTL->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_Tend->pointF(WidgetItem::POINT_TYPE_LEFT));
  d->m_A10=new ArrowItem(d->m_TM0->pointF(WidgetItem::POINT_TYPE_TOP),d->m_UFVB->pointF(WidgetItem::POINT_TYPE_LEFT),ArrowItem::ARROW_TYPE_CORNER);
  d->m_A11=new ArrowItem(d->m_UFVB->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USUM1->pointF(WidgetItem::POINT_TYPE_TOP),ArrowItem::ARROW_TYPE_CORNER,"+");
  d->m_A12=new ArrowItem(d->m_TM0->pointF(WidgetItem::POINT_TYPE_TOP),d->m_UFAB->pointF(WidgetItem::POINT_TYPE_LEFT),ArrowItem::ARROW_TYPE_CORNER);
  d->m_A13=new ArrowItem(d->m_UFAB->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USUM2->pointF(WidgetItem::POINT_TYPE_TOP),ArrowItem::ARROW_TYPE_CORNER,"+");
  d->m_A14=new ArrowItem(d->m_T0->pointF(WidgetItem::POINT_TYPE_LEFT),d->m_UPB->pointF(WidgetItem::POINT_TYPE_RIGHT));
  d->m_A15=new ArrowItem(d->m_UPB->pointF(WidgetItem::POINT_TYPE_LEFT),d->m_USUM0->pointF(WidgetItem::POINT_TYPE_BOTTOM),ArrowItem::ARROW_TYPE_CORNER,"-");

  d->m_scene->addItem(d->m_A0);
  d->m_scene->addItem(d->m_A1);
  d->m_scene->addItem(d->m_A2);
  d->m_scene->addItem(d->m_A3);
  d->m_scene->addItem(d->m_A4);
  d->m_scene->addItem(d->m_A5);
  d->m_scene->addItem(d->m_A6);
  d->m_scene->addItem(d->m_A7);
  d->m_scene->addItem(d->m_A8);
  d->m_scene->addItem(d->m_A9);
  d->m_scene->addItem(d->m_A10);
  d->m_scene->addItem(d->m_A11);
  d->m_scene->addItem(d->m_A12);
  d->m_scene->addItem(d->m_A13);
  d->m_scene->addItem(d->m_A14);
  d->m_scene->addItem(d->m_A15);

  d->m_arrowList.append(d->m_A0);
  d->m_arrowList.append(d->m_A1);
  d->m_arrowList.append(d->m_A2);
  d->m_arrowList.append(d->m_A3);
  d->m_arrowList.append(d->m_A4);
  d->m_arrowList.append(d->m_A5);
  d->m_arrowList.append(d->m_A6);
  d->m_arrowList.append(d->m_A7);
  d->m_arrowList.append(d->m_A8);
  d->m_arrowList.append(d->m_A9);
  d->m_arrowList.append(d->m_A10);
  d->m_arrowList.append(d->m_A11);
  d->m_arrowList.append(d->m_A12);
  d->m_arrowList.append(d->m_A13);
  d->m_arrowList.append(d->m_A14);
  d->m_arrowList.append(d->m_A15);

}

void IGraphPosition::createStartEndTargetItems()
{
  Q_D(IGraphPosition);
  TargetItemWidget *t0=new TargetItemWidget;
  t0->setObjectName("targetItem_posTstart");
  d->m_Tstart=new WidgetItem;
  d->m_Tstart->setWidget(t0);

  t0=new TargetItemWidget;
  t0->setObjectName("targetItem_posTend");
  d->m_Tend=new WidgetItem;
  d->m_Tend->setWidget(t0);

  d->m_scene->addItem(d->m_Tstart);
  d->m_scene->addItem(d->m_Tend);
}

void IGraphPosition::createStartTextItem()
{
  Q_D(IGraphPosition);

  LabelItemWidget *label=new LabelItemWidget(tr("pos cmd"));
  label->setObjectName("label_posStartText");
  d->m_TextStart=new WidgetItem;
  d->m_TextStart->setWidget(label);
  d->m_scene->addItem(d->m_TextStart);
}

void IGraphPosition::createEndTextItem()
{
  Q_D(IGraphPosition);

  LabelItemWidget *label=new LabelItemWidget(tr("cur/torque output"));
  label->setObjectName("label_posEndText");
  label->setAlignment(Qt::AlignRight);
  d->m_TextEnd=new WidgetItem;
  d->m_TextEnd->setWidget(label);
  d->m_scene->addItem(d->m_TextEnd);
}

void IGraphPosition::createAnchorItemHelper()
{
  Q_D(IGraphPosition);
  d->m_anchorHelper=new AnchorItemHelper(this);
}

void IGraphPosition::setUpItemPosAnchors()
{
  Q_D(IGraphPosition);
  d->m_UPID->setPos(PID_POS_X,PID_POS_Y);
  d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USUM0,AnchorItemHelper::AnchorLeft,-1.5*d->m_USUM0->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USUM0,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_USUM0,d->m_TM0,AnchorItemHelper::AnchorLeft,-1*d->m_USUM0->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_USUM0,d->m_TM0,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_TM0,d->m_UIF,AnchorItemHelper::AnchorLeft,-1.2*d->m_UIF->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_TM0,d->m_UIF,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_UIF,d->m_Tstart,AnchorItemHelper::AnchorLeft,-0.7*d->m_UIF->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_UIF,d->m_Tstart,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USATN,AnchorItemHelper::AnchorRight,1.5*d->m_USATN->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USATN,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_USATN,d->m_USUM1,AnchorItemHelper::AnchorRight,1.5*d->m_USUM1->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_USATN,d->m_USUM1,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_USUM1,d->m_UVCTL,AnchorItemHelper::AnchorRight,1.4*d->m_UVCTL->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_USUM1,d->m_UVCTL,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_UVCTL,d->m_USUM2,AnchorItemHelper::AnchorRight,1.6*d->m_USUM2->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_UVCTL,d->m_USUM2,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_USUM2,d->m_UCCTL,AnchorItemHelper::AnchorRight,d->m_UCCTL->boundingRect().width()+d->m_USUM2->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_USUM2,d->m_UCCTL,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_UCCTL,d->m_Tend,AnchorItemHelper::AnchorRight,d->m_UCCTL->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_UCCTL,d->m_Tend,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_UPID,d->m_UFVB,AnchorItemHelper::AnchorHorizontalCenter);
  d->m_anchorHelper->addAnchor(d->m_UPID,d->m_UFVB,AnchorItemHelper::AnchorTop,-1.5*d->m_UFVB->boundingRect().height());

  d->m_anchorHelper->addAnchor(d->m_UFVB,d->m_UFAB,AnchorItemHelper::AnchorHorizontalCenter);
  d->m_anchorHelper->addAnchor(d->m_UFVB,d->m_UFAB,AnchorItemHelper::AnchorTop,-1.5*d->m_UFAB->boundingRect().height());

  d->m_anchorHelper->addAnchor(d->m_UPID,d->m_UPB,AnchorItemHelper::AnchorHorizontalCenter);
  d->m_anchorHelper->addAnchor(d->m_UPID,d->m_UPB,AnchorItemHelper::AnchorBottom,2*d->m_UPB->boundingRect().height());

  d->m_anchorHelper->addAnchor(d->m_UPB,d->m_T0,AnchorItemHelper::AnchorRight,2*d->m_UPB->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_UPB,d->m_T0,AnchorItemHelper::AnchorVerticalCenter);

  d->m_anchorHelper->addAnchor(d->m_Tstart,d->m_TextStart,AnchorItemHelper::AnchorLeft,1.2*d->m_Tstart->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_Tstart,d->m_TextStart,AnchorItemHelper::AnchorBottom,-15);

  d->m_anchorHelper->addAnchor(d->m_Tend,d->m_TextEnd,AnchorItemHelper::AnchorRight,-1.2*d->m_Tend->boundingRect().width());
  d->m_anchorHelper->addAnchor(d->m_Tend,d->m_TextEnd,AnchorItemHelper::AnchorBottom,-15);
}


IGraphPosition::IGraphPosition(IGraphPositionPrivate &dd, QWidget *parent):IGraphWidget(dd,parent)
{

}
