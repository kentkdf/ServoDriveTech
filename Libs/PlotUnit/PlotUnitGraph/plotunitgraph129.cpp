﻿#include "plotunitgraph129.h"
#include "ui_plotunitgraph129.h"
#include "iplotunitgraph_p.h"
#include "Option"
#include "gtutils.h"
#include "sdtglobaldef.h"
#include "sevdevice.h"
#include "tabctlpanel129.h"
#include "plottabctlprms.h"
#include "dialogpickcurve.h"
#include "sdtglobaldef.h"
#include "curvemanager.h"
#include "curvetableaxisitem.h"

#include "pluginsmanager.h"
#include "icurve.h"

#include <QDebug>
#include <QTimer>
#include <QEvent>
#include <QTreeWidgetItem>

#define TEST_DEBUG 1

#define ICON_NAME_ADD       "plot_add.png"
#define ICON_NAME_ALL       "plot_all.png"
#define ICON_NAME_AUTO      "plot_auto.png"
#define ICON_NAME_CLEAR     "plot_clear.png"
#define ICON_NAME_MEAH      "plot_meaH.png"
#define ICON_NAME_MEAV      "plot_meaV.png"
#define ICON_NAME_SOFF      "plot_soff.png"
#define ICON_NAME_SON       "plot_son.png"
#define ICON_NAME_START     "plot_start.png"
#define ICON_NAME_STOP      "plot_stop.png"
#define ICON_NAME_CONFIG    "plot_config.png"
#define ICON_NAME_FIT       "plot_fit.png"
#define ICON_NAME_TAG       "plot_tag.png"
#define ICON_NAME_OPEN      "plot_open.png"
#define ICON_NAME_SAVE      "plot_save.png"
#define ICON_NAME_EXTEND    "plot_extend.png"
#define ICON_NAME_SHRINK    "plot_shrink.png"

#define ICON_NAME_CURVE_ALL    "plot_all.png"
#define ICON_NAME_CURVE_ADD    "plot_add.png"
#define ICON_NAME_CURVE_REMOVE "plot_remove.png"
#define ICON_NAME_CURVE_CLEAR  "plot_clear.png"

#define CURVE_MAX_NUM           16

class VarCurvePrm
{
public:
  int axis;
  CurveVar varPrm;
};

typedef enum {
  COL_TABLE_CURVE_SHOW,
  COL_TABLE_CURVE_NAME,
  COL_TABLE_CURVE_AXIS,
  COL_TABLE_CURVE_DEV,
  COL_TABLE_CURVE_SIZE
}TableCurveColumnInx;

typedef enum{
  ROLE_TABLE_CURVE_ICURVE_PTR = Qt::UserRole+1
}CurveTableDataRole;

//为了QTreeTable 里面包裸ICurve * 指针
Q_DECLARE_METATYPE(ICurve*)

class PlotUnitGraph129Private:public IPlotUnitGraphPrivate
{
  Q_DECLARE_PUBLIC(PlotUnitGraph129)
public:
  PlotUnitGraph129Private():m_pluginManager(NULL),
    m_timer(NULL),
    m_showColor(QColor(Qt::white)),
    m_backShowColor(QColor(Qt::green).darker()),
    m_hideColor(QColor(Qt::white)),
    m_backHideColor(QColor(Qt::gray))
  {

  }
  ~PlotUnitGraph129Private(){}
protected:
  friend class PlotUnitGraph129;
  QList<PlotTabCtlPrms*>m_ctlPrms;//每个设备对应一个
  QList<TabCtlPanel129 *>m_tabCtl;
  QTimer *m_timer;
  PluginsManager *m_pluginManager;
  CurveManager *m_curveManager;
  QColor m_showColor;
  QColor m_backShowColor;
  QColor m_hideColor;
  QColor m_backHideColor;
};

PlotUnitGraph129::PlotUnitGraph129(const QList<SevDevice *> &sevList, QWidget *parent) :
  IPlotUnitGraph(*(new PlotUnitGraph129Private),parent),
  ui(new Ui::PlotUnitGraph129)
{
  Q_D(PlotUnitGraph129);
  d->m_sevList=sevList;

  ui->setupUi(this);
//  connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(onPushButtonTestClicked(bool)));
  ui->splitter_plot_tab->setStretchFactor(0,1);
  ui->splitter_plot_tab->setStretchFactor(1,0);
  ui->splitter_plot_curve->setStretchFactor(0,1);
  ui->splitter_plot_curve->setStretchFactor(1,0);

  ui->tbtn_plot_floatin->setCheckable(true);
  ui->tbtn_plot_floatin->setChecked(true);
  ui->tbtn_plot_startSampling->setCheckable(true);
  ui->tbtn_plot_startSampling->setChecked(false);
  ui->tbtn_plot_mea_horizontal->setCheckable(true);
  ui->tbtn_plot_mea_vertical->setCheckable(true);

  d->m_timer=new QTimer(this);
  d->m_timer->setInterval(1000);

  while(ui->stackedWidget_tabCtlPanel->count()>0)
  {
    QWidget *w=ui->stackedWidget_tabCtlPanel->widget(0);
    ui->stackedWidget_tabCtlPanel->removeWidget(w);
  }

  //加载插件
  bool loadPluginOk=true;
  d->m_pluginManager=new PluginsManager(this);
  loadPluginOk=d->m_pluginManager->loadPlugins();
  if(!loadPluginOk)
    QMessageBox::information(0,tr("error"),tr("load plugin error!\n"));

  d->m_curveManager=new CurveManager(this);

  //曲线表格初始化
  int curveTableWidth = 250;
  int columnCount=3;
  if(sevList.size()>1)
    columnCount=4;
  ui->tableWidget_plot_curve->setColumnCount(columnCount);
  ui->tableWidget_plot_curve->setMinimumWidth(curveTableWidth);
  ui->tableWidget_plot_curve->setEditTriggers(QAbstractItemView::NoEditTriggers);//编辑触发模式
  ui->tableWidget_plot_curve->setAlternatingRowColors(true);
  ui->tableWidget_plot_curve->setContextMenuPolicy(Qt::CustomContextMenu);//设置其右键弹出菜单方法
//  connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onTablePopMenu(QPoint)));
//  m_popuMenuTable=new QMenu(this);
//  addTableMenuAction(mp_userConfig->model.axisCount);//初始化m_popuMenuTable 右键弹出QAction

  int columnWidth=(curveTableWidth-10)/columnCount;
  for(int i=0;i<columnCount;i++)
    ui->tableWidget_plot_curve->horizontalHeader()->resizeSection(i,columnWidth);

  QStringList headerList;
  headerList<<tr("checked")<<tr("name")<<tr("axis");
  ui->tableWidget_plot_curve->setHorizontalHeaderLabels(headerList);
//  ui->tableWidget_plot_curve->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->tableWidget_plot_curve->setMouseTracking(true);
  ui->label_plot_detailName->setMaximumWidth(250);
  ui->label_plot_detailName->setWordWrap(true);
  ui->label_plot_detailName->setAlignment(Qt::AlignTop|Qt::AlignLeft);
//  ui->tableWidget_plot_curve->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

  gtPlotInit();
  ctlPanelInit();

  OptFace *face=dynamic_cast<OptFace *>(OptContainer::instance()->optItem("optface"));
  setPlotIcons(face->css());
  createConnections();
}

PlotUnitGraph129::~PlotUnitGraph129()
{
  delete ui;
}

void PlotUnitGraph129::respondUiActive(bool actived)
{
  Q_D(PlotUnitGraph129);
  d->m_isActiving=actived;
  qDebug()<<"respond Ui active "<<actived;
  setTimerStatus();
}

QColor PlotUnitGraph129::curveShowColor() const
{
  Q_D(const PlotUnitGraph129);
  return d->m_showColor;
}

void PlotUnitGraph129::setCurveShowColor(const QColor &color)
{
  Q_D(PlotUnitGraph129);
  d->m_showColor=color;
}

QColor PlotUnitGraph129::curveBackShowColor() const
{
  Q_D(const PlotUnitGraph129);
  return d->m_backShowColor;
}

void PlotUnitGraph129::setCurveBackShowColor(const QColor &color)
{
  Q_D(PlotUnitGraph129);
  d->m_backShowColor=color;
}

QColor PlotUnitGraph129::curveHideColor() const
{
  Q_D(const PlotUnitGraph129);
  return d->m_hideColor;
}

void PlotUnitGraph129::setCurveHideColor(const QColor &color)
{
  Q_D(PlotUnitGraph129);
  d->m_hideColor=color;
}

QColor PlotUnitGraph129::curveBackHideColor() const
{
  Q_D(const PlotUnitGraph129);
  return d->m_backHideColor;
}

void PlotUnitGraph129::setCurveBackHideColor(const QColor &color)
{
  Q_D(PlotUnitGraph129);
  d->m_backHideColor=color;
}

void PlotUnitGraph129::onSevDeviceListChanged(const QList<SevDevice *> &sevlist)
{
  Q_D(PlotUnitGraph129);
  d->m_sevList=sevlist;
  d->m_timer->stop();
  ctlPanelInit();

  if(sevlist.size()>1)
  {
    QStringList headerList;
    int columnWidth;
    headerList<<tr("checked")<<tr("name")<<tr("axis")<<tr("dev");
    ui->tableWidget_plot_curve->setColumnCount(4);
    ui->tableWidget_plot_curve->setHorizontalHeaderLabels(headerList);
    columnWidth=(ui->tableWidget_plot_curve->minimumWidth()-10)/4;
    for(int i=0;i<4;i++)
      ui->tableWidget_plot_curve->horizontalHeader()->resizeSection(i,columnWidth);
  }
  else
  {
    int columnWidth;
    columnWidth=(ui->tableWidget_plot_curve->minimumWidth()-10)/3;
    ui->tableWidget_plot_curve->setColumnCount(3);
    for(int i=0;i<3;i++)
      ui->tableWidget_plot_curve->horizontalHeader()->resizeSection(i,columnWidth);
  }
}

void PlotUnitGraph129::createConnections()
{
  Q_D(PlotUnitGraph129);
  connect(ui->tbtn_plot_floatin,SIGNAL(clicked(bool)),this,SLOT(onBtnFloatInClicked(bool)));
  OptFace *face=dynamic_cast<OptFace *>(OptContainer::instance()->optItem("optface"));
  connect(face,SIGNAL(faceCssChanged(QString)),this,SLOT(onOptFaceCssChanged(QString)));
  connect(ui->tbtn_plot_mea_horizontal,SIGNAL(clicked(bool)),this,SLOT(onBtnMeaHClicked(bool)));
  connect(ui->tbtn_plot_mea_vertical,SIGNAL(clicked(bool)),this,SLOT(onBtnMeaVClicked(bool)));
  connect(ui->tbtn_plot_fit,SIGNAL(clicked(bool)),this,SLOT(onBtnFitClicked()));
  connect(ui->tbtn_plot_config,SIGNAL(clicked(bool)),this,SLOT(onBtnConfigClicked()));

  connect(ui->plot,SIGNAL(currentPosChanged(QPointF)),this,SLOT(onPlotPosHoverChanged(QPointF)));
  connect(ui->plot,SIGNAL(horizMeaDataChanged(qreal,qreal,qreal)),this,SLOT(onPlotMeaHposChanged(qreal,qreal,qreal)));
  connect(ui->plot,SIGNAL(vertiMeaDataChanged(qreal,qreal,qreal)),this,SLOT(onPlotMeaVposChanged(qreal,qreal,qreal)));

  connect(d->m_timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));

  connect(ui->listWidget_plot_device,SIGNAL(currentRowChanged(int)),this,SLOT(onListWidgetDeviceCurrentRowChanged(int)));

  connect(ui->tbtn_plot_curveAdd,SIGNAL(clicked(bool)),this,SLOT(onBtnCurveAddClicked()));
  connect(ui->tbtn_plot_curveAll,SIGNAL(clicked(bool)),this,SLOT(onBtnCurveShowAllClicked()));
  connect(ui->tbtn_plot_curveClear,SIGNAL(clicked(bool)),this,SLOT(onBtnCurveClearClicked()));
  connect(ui->tbtn_plot_curveRemove,SIGNAL(clicked(bool)),this,SLOT(onBtnCurveRemoveClicked()));
  connect(ui->tableWidget_plot_curve,SIGNAL(itemEntered(QTableWidgetItem*)),this,SLOT(onCurveTableItemEnteredMoreDetail(QTableWidgetItem*)));
  connect(ui->tableWidget_plot_curve,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(onCurveTableItemClicked(QTableWidgetItem*)));
  connect(ui->tableWidget_plot_curve,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(onCurveTableItemDoubleClicked(QTableWidgetItem*)));
  //  //tab mode

//  //tab motion
}
void PlotUnitGraph129::onBtnFloatInClicked(bool checked)
{
  Q_UNUSED(checked);
  Q_D(PlotUnitGraph129);
  d->m_winIsIn=!d->m_winIsIn;
  setTimerStatus();
  emit winFloatingChange(d->m_winIsIn);
}

void PlotUnitGraph129::onOptFaceCssChanged(const QString &css)
{
  Q_D(PlotUnitGraph129);

  setPlotIcons(css);

  for(int i=0;i<d->m_tabCtl.size();i++)
  {
    d->m_tabCtl.at(i)->setupIcons(css);
  }
}

void PlotUnitGraph129::onBtnMeaHClicked(bool checked)
{
  if(checked)
    ui->plot->createHorizMea();
  else
    ui->plot->clearHorizMea();
}

void PlotUnitGraph129::onBtnMeaVClicked(bool checked)
{
  if(checked)
    ui->plot->createVertiMea();
  else
    ui->plot->clearVertiMea();
}

void PlotUnitGraph129::onBtnFitClicked()
{
  ui->plot->rescaleAxes();
  ui->plot->replot();
}

void PlotUnitGraph129::onBtnConfigClicked()
{
  bool checked=ui->tbtn_plot_startSampling->isChecked();
  ui->tbtn_plot_startSampling->setChecked(!checked);
}

void PlotUnitGraph129::onPlotPosHoverChanged(const QPointF &point)
{
  ui->label_plot_x->setText(QString("X=%1 ms").arg(QString::number(point.x()*1000,'f',2)));
  ui->label_plot_y->setText(QString(" Y=%1").arg(QString::number(point.y(),'f',2)));
}

void PlotUnitGraph129::onPlotMeaVposChanged(qreal v1, qreal v2, qreal dv)
{
  Q_UNUSED(v1);
  Q_UNUSED(v2);
  ui->label_plot_dy->setText(QString(" dY=%1").arg(QString::number(dv,'f',2)));
}

void PlotUnitGraph129::onPlotMeaHposChanged(qreal v1, qreal v2, qreal dv)
{
  Q_UNUSED(v1);
  Q_UNUSED(v2);
  ui->label_plot_dx->setText(QString("dX=%1 ms ,Freq=%2 HZ").arg(QString::number(dv*1000,'f',2)).arg(QString::number(1/dv,'f',2)));
}

void PlotUnitGraph129::onTimeOut()
{
  Q_D(PlotUnitGraph129);
  static quint32 i=0;
  //qDebug()<<"time out"<<i;
  i++;
}

void PlotUnitGraph129::onListWidgetDeviceCurrentRowChanged(int row)
{
  Q_D(PlotUnitGraph129);
  d->m_curSevInx=row;
  qDebug()<<"updateCtlPanelBySelectDevice"<<row;
  updateCtlPanelBySelectDevice(row);
}

void PlotUnitGraph129::onBtnCurveAddClicked()
{
  Q_D(PlotUnitGraph129);
  DialogPickCurve *dia=new DialogPickCurve();
  SevDevice *dev=d->m_sevList.at(d->m_curSevInx);

  QTreeWidget *treeExpert=dev->axisTreeSource(0,"RAM");
  dia->addExpertTreeWidget(treeExpert);
  dia->setAxisTable(dev->axisNum());

  connect(dia,SIGNAL(expertTreeItemDoubleClicked(QTableWidget*,QTreeWidgetItem*)),this,SLOT(onExpertTreeWidgetDoubleClicked(QTableWidget*,QTreeWidgetItem*)));

  dia->exec();
  delete dia;
}

void PlotUnitGraph129::onExpertTreeWidgetDoubleClicked(QTableWidget *table,QTreeWidgetItem *item)
{
  if(item->childCount()>0) return;//不是叶子则返回
  int isBit;
  QString name;
//  QList<int>axisList;
//  QList<CurveVar> varPrmList;
  QList<VarCurvePrm>varPrmList;
  name=item->text(GT::COL_FR_NAME);

  isBit=item->text(GT::COL_FR_ISBIT).toInt();
  if(isBit==1)//判断是不是位，如果是位就不增加
  {
    QMessageBox::information(0,tr("warning"),tr("%1 is BIT").arg(name));
    return;
  }
  //找 bytes axisInx(more then one) offsetAddr

  for(int i=0;i<table->columnCount();i++)
  {
    if(table->item(0,i)->isSelected())
    {
      VarCurvePrm var;
      var.axis=i;
      var.varPrm.keyName=name;
      var.varPrm.prm.bytes=GTUtils::byteNumbers(item->text(GT::COL_FR_TYPE));
      var.varPrm.prm.baseAddr=0;
      var.varPrm.prm.offtAddr=item->text(GT::COL_FR_ADDRESS).toUShort();
      varPrmList.append(var);
    }
  }

  int axisInx;
  //检查该曲线名在对应的轴中有没有这个变量
  for(int i=0;i<varPrmList.count();i++)
  {
    axisInx=varPrmList.at(i).axis;
    if(axisInx%2!=0)//0 2 4不检查 1 3 5检查
    {
      QTreeWidget *ramTree=currentSevDevice()->axisTreeSource(axisInx,"RAM");
      QTreeWidgetItem *item=GTUtils::findItem(name,ramTree,GT::COL_FR_NAME);
      if(NULL==item)
      {
        varPrmList.removeAt(i);
        qDebug()<<"can not find curve :"<<name<<"in axis "<<axisInx;
      }
      else//更新地址
      {
        varPrmList[i].varPrm.prm.offtAddr=item->text(GT::COL_FR_ADDRESS).toUShort();
      }
      qDebug()<<"axis "<<axisInx<<"find item"<<item->text(GT::COL_FR_NAME);
    }
  }

  //判断 下放曲线条数是否大于16条，将多余的移除
  Q_D(PlotUnitGraph129);
  int totalCount;
  totalCount=d->m_curveManager->varCurveTotalCount()+varPrmList.count();
  int dCount=totalCount-CURVE_MAX_NUM;
  bool isOver=false;
  while(dCount>0)
  {
    isOver=true;
    varPrmList.removeLast();
    dCount--;
  }
  if(isOver)
    QMessageBox::information(0,tr("warning"),tr("The numbers of selected curves is too much"));



#if TEST_DEBUG
  for(int i=0;i<varPrmList.count();i++)
  {
    qDebug()<<"add curve axis="<<varPrmList.at(i).axis;
    qDebug()<<"bytes"<<varPrmList.at(i).varPrm.prm.bytes<<" offset"<<varPrmList.at(i).varPrm.prm.offtAddr<<" base"<<varPrmList.at(i).varPrm.prm.baseAddr;
  }
#endif


  ICurve *curve;
  int curveCount=d->m_curveManager->curveList().count();
  int curveTotalSize;
  for(int i=0;i<varPrmList.count();i++)
  {
    axisInx=varPrmList.at(i).axis;
    curveTotalSize=curveCount+i;
    //生成曲线对象
    curve=d->m_pluginManager->expertCurve()->clone();
    curve->prepare();
    curve->setAxisInx(axisInx);
    curve->setDevInx(d->m_curSevInx);
    curve->setName(name);
    curve->setNote("");
    curve->setRowInx(curveTotalSize);
    curve->setSamplInterval(62.5);//这里还要从Option-plot里读取
    curve->setStorePointCount(10/62.5*1000000);//这里还要从Option-plot里读取
    curve->setColor(d->m_curveManager->color(curveTotalSize));
    curve->addVarInputByName(name);
    curve->fillVarInputsPrm(0,varPrmList.at(i).varPrm.prm);
    d->m_curveManager->addCurve(curve);
    //表格增加曲线标识
    addTableRowPrm(curve);
    //gtplot增加曲线
    ui->plot->addGraph();
    ui->plot->graph(curveTotalSize)->setPen(curve->color());
  }

  qDebug()<<"curve total size ="<<d->m_curveManager->curveList().size();

}

void PlotUnitGraph129::onBtnCurveRemoveClicked()
{
  int row=ui->tableWidget_plot_curve->currentRow();
  if(row>=0)
  {
    Q_D(PlotUnitGraph129);
    ui->plot->removeGraph(row);
    d->m_curveManager->removeCurve(row);
    ui->tableWidget_plot_curve->removeRow(row);
    ui->plot->replot();
  }
}

void PlotUnitGraph129::onBtnCurveClearClicked()
{
  Q_D(PlotUnitGraph129);
  if(d->m_curveManager->curveList().size()>0)
  {
    d->m_curveManager->clearCurve();
    ui->plot->clearGraphs();
    ui->tableWidget_plot_curve->setRowCount(0);
    ui->tableWidget_plot_curve->clearContents();
    ui->plot->replot();
  }
}

void PlotUnitGraph129::onBtnCurveShowAllClicked()
{

}

void PlotUnitGraph129::onCurveTableItemClicked(QTableWidgetItem *item)
{
  if(item->column() == COL_TABLE_CURVE_SHOW)
  {
    Q_D(PlotUnitGraph129);
    int row = item->row();
    QString str;
    QColor color;
    ICurve *curve=item->data(ROLE_TABLE_CURVE_ICURVE_PTR).value<ICurve *>();
    if(curve->isDraw())
    {
      str=tr("off");
      color=d->m_backHideColor;
      curve->setIsDraw(false);
    }
    else
    {
      str=tr("on");
      color=d->m_backShowColor;
      curve->setIsDraw(true);
    }
    item->setText(str);
    item->setBackgroundColor(color);
    ui->plot->graph(row)->setVisible(curve->isDraw());
    ui->plot->replot();
  }
}

void PlotUnitGraph129::onCurveTableItemDoubleClicked(QTableWidgetItem *item)
{
  if(item->column()==COL_TABLE_CURVE_NAME)
  {
    int row=item->row();
    ICurve *curve=item->data(ROLE_TABLE_CURVE_ICURVE_PTR).value<ICurve *>();
    QColor clr=curve->color();
    clr=QColorDialog::getColor(clr);
    if(clr.isValid())
    {
      curve->setColor(clr);
      item->setTextColor(clr);
      ui->plot->graph(row)->setPen(QPen(clr));
      ui->plot->replot();
      qDebug()<<"double clicked -------------select color"<<clr;
    }
  }
}

void PlotUnitGraph129::onCurveTableItemEnteredMoreDetail(QTableWidgetItem * item)
{
  if(item->column() == COL_TABLE_CURVE_NAME)
  {
    ICurve *curve=item->data(ROLE_TABLE_CURVE_ICURVE_PTR).value<ICurve *>();
    ui->label_plot_detailName->setText(curve->fullName());
  }
}

void PlotUnitGraph129::setPlotIcons(const QString &css)
{
  QSize iconSize(24,24);
  QString iconPath=GTUtils::customPath()+"option/style/"+css+"/icon/";
  QIcon samplStartIcon;
  samplStartIcon.addPixmap(QPixmap(iconPath+ICON_NAME_START),QIcon::Selected,QIcon::Off);
  samplStartIcon.addPixmap(QPixmap(iconPath+ICON_NAME_STOP),QIcon::Selected,QIcon::On);
  ui->tbtn_plot_startSampling->setIcon(samplStartIcon);
  ui->tbtn_plot_startSampling->setIconSize(iconSize);

  QIcon floatIcon;
  floatIcon.addPixmap(QPixmap(iconPath+ICON_NAME_EXTEND),QIcon::Selected,QIcon::On);
  floatIcon.addPixmap(QPixmap(iconPath+ICON_NAME_SHRINK),QIcon::Selected,QIcon::Off);
  ui->tbtn_plot_floatin->setIcon(floatIcon);
  ui->tbtn_plot_floatin->setIconSize(iconSize);

  ui->tbtn_plot_config->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_CONFIG)));
  ui->tbtn_plot_config->setIconSize(iconSize);

  ui->tbtn_plot_auto->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_AUTO)));
  ui->tbtn_plot_auto->setIconSize(iconSize);

  ui->tbtn_plot_fit->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_FIT)));
  ui->tbtn_plot_fit->setIconSize(iconSize);

  ui->tbtn_plot_tag->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_TAG)));
  ui->tbtn_plot_tag->setIconSize(iconSize);

  ui->tbtn_plot_mea_horizontal->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_MEAH)));
  ui->tbtn_plot_mea_horizontal->setIconSize(iconSize);

  ui->tbtn_plot_mea_vertical->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_MEAV)));
  ui->tbtn_plot_mea_vertical->setIconSize(iconSize);

  ui->tbtn_plot_open->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_OPEN)));
  ui->tbtn_plot_open->setIconSize(iconSize);

  ui->tbtn_plot_save->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_SAVE)));
  ui->tbtn_plot_save->setIconSize(iconSize);

  ui->tbtn_plot_curveAll->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_CURVE_ALL)));
  ui->tbtn_plot_curveAll->setIconSize(iconSize);

  ui->tbtn_plot_curveAdd->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_CURVE_ADD)));
  ui->tbtn_plot_curveAdd->setIconSize(iconSize);

  ui->tbtn_plot_curveRemove->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_CURVE_REMOVE)));
  ui->tbtn_plot_curveRemove->setIconSize(iconSize);

  ui->tbtn_plot_curveClear->setIcon(QIcon(QPixmap(iconPath+ICON_NAME_CURVE_CLEAR)));
  ui->tbtn_plot_curveClear->setIconSize(iconSize);

//  qDebug()<<"PlotUnitGraph129 css changed"<<css<<iconPath;
}

void PlotUnitGraph129::gtPlotInit()
{
  setupSimpleDemo(ui->plot);
  ui->plot->xAxis2->setVisible(false);
  ui->plot->xAxis2->setTickLabels(false);
  ui->plot->yAxis2->setVisible(false);
  ui->plot->yAxis2->setTickLabels(false);
  ui->plot->setBackground(QBrush(QColor(240,240,240)));
}

void PlotUnitGraph129::ctlPanelInit()
{
  Q_D(PlotUnitGraph129);

  GT::deepClearList(d->m_tabCtl);
  ui->listWidget_plot_device->clear();

  for(int i=0;i<d->m_sevList.size();i++)
  {
    TabCtlPanel129 *panel=new TabCtlPanel129(d->m_sevList.at(i));
    d->m_tabCtl.append(panel);
    ui->stackedWidget_tabCtlPanel->addWidget(panel);
    ui->listWidget_plot_device->addItem(d->m_sevList.at(i)->aliasName());
  }
  d->m_curSevInx=0;
  ui->listWidget_plot_device->setCurrentRow(d->m_curSevInx);
  bool isShow=(d->m_sevList.count()>1);
  ui->widget_plot_device_nav->setVisible(isShow);
}

void PlotUnitGraph129::updateCtlPanelBySelectDevice(int sevInx)
{
  Q_D(PlotUnitGraph129);
  if(!(sevInx<d->m_sevList.size())||(sevInx==-1))
    return;

  ui->stackedWidget_tabCtlPanel->setCurrentIndex(sevInx);
}

void PlotUnitGraph129::setupSimpleDemo(QCustomPlot *customPlot)
{
  // add two new graphs and set their look:
  customPlot->addGraph();
  customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
  customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
  customPlot->addGraph();
  customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
  // generate some points of data (y0 for first, y1 for second graph):
  QVector<double> x(251), y0(251), y1(251);
  for (int i=0; i<251; ++i)
  {
    x[i] = i;
    y0[i] = 100*qExp(-i/150.0)*qCos(i/10.0); // exponentially decaying cosine
    y1[i] = 100*qExp(-i/150.0);              // exponential envelope
  }
  // configure right and top axis to show ticks but no labels:
  // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
  customPlot->xAxis2->setVisible(true);
  customPlot->xAxis2->setTickLabels(false);
  customPlot->yAxis2->setVisible(true);
  customPlot->yAxis2->setTickLabels(false);
  // make left and bottom axes always transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
  // pass data points to graphs:
  customPlot->graph(0)->setData(x, y0);
  customPlot->graph(1)->setData(x, y1);
  // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
  customPlot->graph(0)->rescaleAxes();
  // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
  customPlot->graph(1)->rescaleAxes(true);
  // Note: we could have also just called customPlot->rescaleAxes(); instead
  // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );

  customPlot->setSelectionRectMode(QCP::srmNone);

  QCPItemStraightLine *line=new QCPItemStraightLine(customPlot);
  line->setSelectable(true);
  customPlot->setInteraction(QCP::iSelectItems,true);
  customPlot->setInteraction(QCP::iMultiSelect,true);

  QCPItemText *text=new QCPItemText(customPlot);
  text->setText("hello");
  text->position->setType(QCPItemPosition::ptPlotCoords);
  text->position->setCoords(50,100);
}

void PlotUnitGraph129::setTimerStatus()
{
  Q_D(PlotUnitGraph129);
  bool needOpen;
  needOpen=d->m_isActiving||(!d->m_winIsIn);
  qDebug()<<"needOpen = "<<needOpen;
  if(needOpen)
  {
    if(d->m_timer->isActive()==false)
      d->m_timer->start();
  }
  else
  {
    d->m_timer->stop();
  }
}

SevDevice *PlotUnitGraph129::currentSevDevice() const
{
  Q_D(const PlotUnitGraph129);
  return d->m_sevList.at(d->m_curSevInx);
}

void PlotUnitGraph129::addTableRowPrm(ICurve *curve)
{
  Q_D(PlotUnitGraph129);

  int rowCount=ui->tableWidget_plot_curve->rowCount();
  ui->tableWidget_plot_curve->insertRow(rowCount);

  QString str;
  QColor color;

  for (int nCol = 0; nCol < COL_TABLE_CURVE_SIZE; nCol++)
  {

    QTableWidgetItem *item = new QTableWidgetItem();
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    switch (nCol)
    {
    case COL_TABLE_CURVE_SHOW:
    {
      if(curve->isDraw())
      {
        str=tr("on");
        color=d->m_backShowColor;
        item->setTextColor(d->m_showColor);
      }
      else
      {
        str=tr("off");
        color=d->m_backHideColor;
        item->setTextColor(d->m_hideColor);
      }
      item->setBackgroundColor(color);
    }
    break;

    case COL_TABLE_CURVE_NAME:
    {
      str=curve->displayName();
      color=curve->color();
      item->setTextColor(color);
    }
    break;

    case COL_TABLE_CURVE_AXIS:
    {
      str=QString::number(curve->axisInx());
    }
    break;

    case COL_TABLE_CURVE_DEV:
    {
      str=curve->devName();
    }
    break;

    default:
    break;
    }
    QVariant v;
    v.setValue(curve);
    item->setData(ROLE_TABLE_CURVE_ICURVE_PTR,v);
    item->setText(str);
    ui->tableWidget_plot_curve->setItem(rowCount, nCol, item);
  }
}

