﻿#ifndef SEVDEVICE_H
#define SEVDEVICE_H

#include <QObject>
#include <QList>
#include "sdkernel_global.h"
#include "sevpwrboard.h"
#include "icom.h"
#include "sdtglobaldef.h"

namespace ComDriver {
class ICom;
}

//using namespace ComDriver;

class DeviceConfig;
class LinkSocket;
class ICtrBoard;
class IPwrBoard;
class IDspMap;
class SevDevicePrivate;
class QTreeWidgetItem;
class QTreeWidget;

class SDKERNELSHARED_EXPORT SevDevice : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(SevDevice)

public:
  explicit SevDevice(QObject *parent=0);
  ~SevDevice();
  bool init(const DeviceConfig *dConfig);

  bool adjustSocket(void (*processCallBack)(void *argv, short *value), void *uiProcessBar);
  ComDriver::ICom *socketCom() const;
  quint8 comType() const;

  bool enableConnection(void (*processCallBack)(void *argv, short *value), void *uiProcessBar);
  void disableConnection();
  bool isConnecting() const;
  bool isOffline() const;
  void setOfflineStatus(bool mode);

  bool containsCmd(const QString &cmdKey);

  quint64 genCmdRead(const QString &cmdReadName,qint16 axisIndex,bool &isOk);
  bool genCmdWrite(const QString &cmdWriteName,quint64 value,qint16 axisIndex);

  bool readPageFlash(int axis,QTreeWidget *pageTree);
  bool writePageFlash(int axis,QTreeWidget *pageTree);

  bool readGenItemRAM(quint16 axisInx,QTreeWidgetItem *item);
  bool writeGenItemRAM(quint16 axisInx,QTreeWidgetItem *item);
  bool readGenPageRAM(quint16 axisInx,QTreeWidget *pageTree);
  bool writeGenPageRAM(quint16 axisInx,QTreeWidget *pageTree);
  
  bool readOffLinePagePrm(quint16 axisInx, QTreeWidget *pageTree);
  bool writeOffLinePagePrm(quint16 axisInx, QTreeWidget *pageTree);
  
  bool readOfflineAdvItem(quint16 axisInx,QTreeWidgetItem *item);
  bool writeOfflineAdvItem(quint16 axisInx,QTreeWidgetItem *item);

  bool readOffLinePrmByAddr(quint16 axisInx, quint16 offset, double &value);
  bool writeOffLinePrmByAddr(quint16 axisInx, quint16 offset, double &value);

  bool writePageItemFlash(quint16 axisInx,QTreeWidgetItem *item);
  bool readPageItemFlash(quint16 axisInx,QTreeWidgetItem *item);

  bool writeAdvFlash(quint16 axisInx, QTreeWidgetItem *item);
  bool writeAdvRam(quint16 axisInx, QTreeWidgetItem *item);

  bool readAdvFlash(quint16 axisInx, QTreeWidgetItem *item);
  bool readAdvRam(quint16 axisInx, QTreeWidgetItem *item);
  bool readAdvRam(quint16 axisInx, quint16 offset, quint16 base, int bytesNum , double &result);

  bool startPlot(const ComDriver::PlotControlPrm &ctrPrm);
  bool stopPlot(const ComDriver::PlotControlPrm &ctrPrm);
  bool getPlotData(const ComDriver::PlotControlPrm &ctrPrm,ComDriver::CurveList &curveList);

  bool clearAlarm(quint16 axisInx);
  bool checkLoadParameters(QTreeWidget *tree, int itemNum);

  bool writeXml(quint8 axis, const QStringList &fileNameList, QList<int> fileTypeList, int file_num, void (*processCallBack)(void *, short *), void *ptrv, short &progress);
  bool readXml(quint8 axis, const QStringList &fileNameList, QList<int> fileTypeList, int file_num, void (*processCallBack)(void *, short *), void *ptrv, short &progress);

  QString aliasName() const;
  QString typeName() const;
  QString modelName() const;
  QString versionName()const;

  quint32 devId() const;
  void resetDevId(quint32 id);
  quint32 pwrId() const ;
  quint32 ctrId() const;
  QString fpgaId() const ;
  quint8 axisNum() const;
  quint8 stationId() const;

  DeviceConfig *deviceConfig() const;
  QTreeWidgetItem* targetTree() const;
  QTreeWidget *axisTreeSource(int axis,int page) const;
  QTreeWidget *axisTreeSource(int axis,const QString&name) const;
  QTreeWidget *globalTreeSource(int page) const;
  QTreeWidget *offlineTree();

  void setVersionAttributeActive();

  bool checkNetStatus();

  QString filePath() const;

  Q_INVOKABLE void qmlTest();

  //writePrm readPrm读写的是指所有个轴的驱动器参数文件
  bool writePrmItemFlash(quint16 axisInx, QTreeWidgetItem *item);
  bool readPrmItemFlash(quint16 axisInx, QTreeWidgetItem *item);

  bool axisBrakeIsOn(quint16 axisInx);
  void setBrakeMarkOn(quint16 axisInx, bool enable);

  bool axisServoIsOn(quint16 axisInx);
  void setAxisServoOn(quint16 axisInx, bool enable);

  int currentTaskServoMode(quint16 axisInx);
  void setCurrentTaskServoMode(quint16 axisInx,int mode);
  void setControlSrc(quint16 axisInx ,GT::SevControlSrc ctlSrc);
  GT::SevControlSrc controlSrc(quint16 axisInx);

  void cmdSetPosAdjRef(quint16 axisInx,double value);
  void cmdSetUaRef(quint16 axisInx,double value);
  void cmdSetUbRef(quint16 axisInx,double value);
  void cmdSetUcRef(quint16 axisInx,double value);
  void cmdSetUdRef(quint16 axisInx,double value);
  void cmdSetUqRef(quint16 axisInx,double value);
  void cmdSetIdRef(quint16 axisInx,double value);
  void cmdSetIqRef(quint16 axisInx,double value);
  void cmdSetSpdRef(quint16 axisInx,double value);
  void cmdSetPosRef(quint16 axisInx,qint32 value);
  bool cmdGetSpdFbPercent(quint16 axisInx,double &value);

  bool searchPhaseStart(quint16 axisInx,int value);

  bool imaxPrmAssociationActive(quint16 axisInx);
  bool resetDSP();

  bool checkPageParameters(int axis,QTreeWidget *tree);

  quint64 genCmdReadNos(int axisInx,bool &isOk);
  quint64 genCmdReadAutoTurnningFgd(int axisInx,bool &isOk);//103
  quint64 genCmdReadAutoTurnningFgp(int axisInx,bool &isOk);//104
  quint64 genCmdReadAutoTurnningFgi(int axisInx,bool &isOk);//105
  quint64 genCmdReadAutoTurnningFgn(int axisInx,bool &isOk);//106
  bool genCmdAutoTurnningFinishFlag(int axisInx,bool &isOk);//107

  bool genCmdWritePlanSpdMax(int axisInx,quint64 value);//77
  bool genCmdWritePlanSpdAcc(int axisInx,quint64 value);//78
  bool genCmdWritePlanSpdDec(int axisInx,quint64 value);//79

  bool writeGearPrm(quint16 axisInx,qint32 a,qint32 b);
  bool readGearPrm(quint16 axisInx,qint32 &a,qint32 &b);

  bool writePulseGearPrm(quint16 axisInx,qint32 c,qint32 d2);
  bool readPulseGearPrm(quint16 axisInx,qint32 &c,qint32 &d2);

  bool readAlarmHistoryIndex(quint16 axisInx, qint16 &index);
  bool readAlarmHistoryList(quint16 axisInx, QList<qint32> &alarmList);

  bool readFLASH16ByAddr(quint16 axisInx, quint16 addr, quint16 &value);
  bool writeFLASH16ByAddr(quint16 axisInx, quint16 addr, quint16 value);


signals:
  void initProgressInfo(int value,QString msg);
  void itemRangeValid(QTreeWidgetItem *item,int status);
  void alarmError(quint16 devId,quint16 axisInx,bool hasError);//当checkStatus时，对外发送状态信息
  void netError(quint16 axisInx);
  void connectionChanged(bool isConnected);

  //寻相相关
  void ipaSearchPhaseInfo(int barValue, const QString &msg);
  void ipaWarningMsg(const QString &msg);
  void ipaDone();

  void dspReset();

public slots:


private:
  bool checkPagePropertyParameters(QTreeWidgetItem *item);
  bool checkPowerBoardParameters(QTreeWidgetItem *item,const QMap<QString ,PowerBoardLimit> *limit);

  bool checkLoadItemParameters(int axis, QTreeWidgetItem *item, QTreeWidget *prmTree, int itemNum);
  bool checkLoadPropertyParameters(QTreeWidgetItem *item, QTreeWidget *prmTree);

private:
  SevDevicePrivate *d_ptr;
  friend class ImaxPrmAssociationHelper;
};

#endif // SEVDEVICE_H
