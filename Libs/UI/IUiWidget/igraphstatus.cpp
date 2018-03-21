﻿#include "igraphstatus.h"
#include "igraphstatus_p.h"

#include <QGridLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QMenu>
#include <QLabel>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QStyleOption>

#define ALM_CODE_ALL_INX 3

LedAlarm::LedAlarm(const QString &name, QWidget *parent, quint16 id, LedTextPosition pos) : QWidget(parent),
  m_id(id),
  m_label(new QLabel(name,this)),
  m_menuActive(false),
  m_led(new LedAlarm::Led(this))
{
  QLayout *layout=NULL;
  if(pos==LED_TEXT_BOTTOM)
  {
    layout=new QVBoxLayout(this);
  }
  else
  {
    layout=new QHBoxLayout(this);
  }
  layout->addWidget(m_led);
  layout->addWidget(m_label);
  setLayout(layout);
}

LedAlarm::~LedAlarm()
{
}

void LedAlarm::setLedName(const QString &name)
{
  m_label->setText(name);
}

void LedAlarm::addMenuAction(QAction *action)
{
  m_led->menu()->addAction(action);
}

bool LedAlarm::menuActive() const
{
  return m_menuActive;
}

void LedAlarm::setMenuActive(bool active)
{
  m_menuActive = active;
  m_led->menu()->setEnabled(active);
}

void LedAlarm::setError(bool error)
{
  m_led->setError(error);
}


LedAlarm::Led::Led(LedAlarm *parent):QPushButton(parent),
  m_menu(new QMenu(this)),
  m_parent(parent),
  m_passColor(Qt::green),
  m_errorColor(Qt::red),
  m_hasError(false)
{
  setMenu(m_menu);
  setStyleSheet("background-color:yellow");
  setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));
}

LedAlarm::Led::~Led()
{

}

QSize LedAlarm::Led::sizeHint() const
{
  int width=fontMetrics().width("m")*2;
  return QSize(width,width);
}

void LedAlarm::Led::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  QStyleOption opt;
  opt.init(this);
  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
  painter.setRenderHint(QPainter::Antialiasing,true);
  QPen pen;
  pen.setWidth(2);
  QColor color;
  if(m_hasError)
    color=m_errorColor;
  else
    color=m_passColor;
  pen.setColor(color);
//  painter.setPen(pen);
  painter.setBrush(QBrush(color));

  QColor frameColor=color.lighter();

  int w=width();
  qreal adjust=pen.width()/2;
  painter.drawEllipse(0+adjust,0+adjust,w-adjust*2,w-adjust*2);

  pen.setColor(frameColor);
  painter.setPen(pen);
  painter.drawEllipse(0+adjust,0+adjust,w-adjust*2,w-adjust*2);
}

QColor LedAlarm::Led::errorColor() const
{
  return m_errorColor;
}

void LedAlarm::Led::setErrorColor(const QColor &errorColor)
{
  m_errorColor = errorColor;
  emit errorColorChanged(m_errorColor);
}

bool LedAlarm::Led::hasError() const
{
  return m_hasError;
}

void LedAlarm::Led::setError(bool error)
{
  if(m_hasError!=error)
  {
    m_hasError=error;
    update();
  }
}

QColor LedAlarm::Led::passColor() const
{
  return m_passColor;
}

void LedAlarm::Led::setPassColor(const QColor &passColor)
{
  m_passColor = passColor;
  emit passColorChanged(m_passColor);
}



IGraphStatusPrivate::IGraphStatusPrivate()
{

}

IGraphStatusPrivate::~IGraphStatusPrivate()
{

}

//IGraphStatus::IGraphStatus(QWidget *parent) : IGraph(*(new IGraphStatusPrivate),parent)
//{

//}

IGraphStatus::~IGraphStatus()
{

}

void IGraphStatus::visit(IUiWidget *uiWidget)
{
  initPrivateData(uiWidget);
  setUiVersionName();
  setCustomVisitActive(uiWidget);
  setupDataMappings();
  setCommonConnections();
}

void IGraphStatus::setCustomVisitActive(IUiWidget *uiWidget)
{
  Q_D(IGraphStatus);

  Q_UNUSED(uiWidget);
  QWidget *widget=alarmBackgroundWidget();
  if(!widget)
    return;

  QGridLayout *gridLayout=new QGridLayout(widget);
  widget->setLayout(gridLayout);
  QTreeWidgetItem *almCodeAllItem=d->m_treeWidget->topLevelItem(ALM_CODE_ALL_INX);
  int count=almCodeAllItem->childCount();

  int col=3;
  int row=count/3;
  int remain=count%3;
  int rsv=0;
  if(remain>0)
    col++;
  rsv=col-remain;

  int addCount=0;
  int i=0;
  int j=0;
  for(i=0;i<col;col++)
  {
    for(j=0;j<row;j++)
    {
      QLabel *label=new QLabel(this);
      label->setText(tr("row=%1 col=%2").arg(row).arg(j));
      gridLayout->addWidget(label,j,i);
      addCount++;
      if(addCount>=count)
        break;
    }
  }
  for(int k=0;k<rsv;k++)
  {
    QLabel *label=new QLabel(this);
    label->setText(tr("row=%1 col=%2").arg(row).arg(j));
    gridLayout->addWidget(label,j+k,i);
  }

}

void IGraphStatus::onFaceCssChanged(const QString &css)
{
  setDeviceStatusIconByCss(css);
}

IGraphStatus::IGraphStatus(IGraphStatusPrivate &dd, QWidget *parent):IGraph(dd,parent)
{

}
