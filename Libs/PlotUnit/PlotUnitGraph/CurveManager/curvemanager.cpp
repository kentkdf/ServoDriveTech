﻿#include "curvemanager.h"
#include "icurve.h"
#include "gtutils.h"
#include "sdtglobaldef.h"

#include <QMultiHash>
#include <QDebug>

#define CURVE_MANAGER_TEST 1

CurveManager::CurveManager(QObject *parent) : QObject(parent),
  m_sampleScale(1)
{
  //----设置好默认颜色------------
  m_colorDefault.append(QColor(Qt::red));
  m_colorDefault.append(QColor(Qt::green));
  m_colorDefault.append(QColor(Qt::blue));
  m_colorDefault.append(QColor(Qt::black));

  m_colorDefault.append(QColor(255,0,255));
  m_colorDefault.append(QColor(0,170,0));
  m_colorDefault.append(QColor(0,170,255));
  m_colorDefault.append(QColor(170,0,255));
  m_colorDefault.append(QColor(170,0,0));
  m_colorDefault.append(QColor(255,170,127));
}

CurveManager::~CurveManager()
{

}

QList<ICurve *> CurveManager::curveList() const
{
  return m_curveList;
}

void CurveManager::addCurve(ICurve *curve)
{
  m_curveList.append(curve);
}

void CurveManager::removeCurve(ICurve *curve)
{
  m_curveList.removeOne(curve);
  delete curve;
}

void CurveManager::removeCurve(int inx)
{
  if(inx<m_curveList.count())
  {
    ICurve *curve=m_curveList.at(inx);
    m_curveList.removeAt(inx);
    delete curve;
  }
}

void CurveManager::clearCurve()
{
  GT::deepClearList(m_curveList);
}

int CurveManager::varCurveTotalCount()
{
  int num=0;
  ICurve *curve;
  for(int i=0;i<m_curveList.count();i++)
  {
    curve=m_curveList.at(i);
    num+=curve->varInputsKeys().size();
  }
  return num;
}

QColor CurveManager::color(int totalCount) const
{
  return m_colorDefault.at(totalCount%m_colorDefault.size());
}

QList<DevSamplePrm> CurveManager::samplPrms()
{
  m_samplPrms.clear();

  //找到采样的设备ID
  QList<int> devs;
  for(int i=0;i<m_curveList.size();i++)
  {
    int dev=m_curveList.at(i)->devInx();
    if(devs.isEmpty())
    {
      devs.append(dev);
      continue;
    }
    else
    {
      bool has=false;
      for(int j=0;j<devs.size();j++)
      {
        if(dev==devs.at(j))
        {
          has=true;
          break;
        }
      }
      if(has==false)
        devs.append(dev);
    }
  }

  //设备归类
  QMultiHash<int ,ICurve *> multiHashDev;
  for(int i=0;i<m_curveList.size();i++)
  {
    ICurve *curve=m_curveList.at(i);
    multiHashDev.insert(curve->devInx(),curve);
  }
  qDebug()<<"device total size"<<devs.size()<<"hashDev value(0) size"<<multiHashDev.values(0).size();

  for(int i=0;i<devs.size();i++)
  {
    QList<ICurve *>curves;
    ICurve *c;
    DevSamplePrm devPrm;
    devPrm.m_devInx=devs.at(i);
    curves=multiHashDev.values(devPrm.m_devInx);
    qDebug()<<"at device "<<devPrm.m_devInx<<"curves total size"<<curves.size();

    //找出DSP 0 1 2
    QList<int> dsps;
    for(int i=0;i<curves.size();i++)
    {
      int d=curves.at(i)->dspInx();
      qDebug()<<"ddddddddddddddddddd "<<d;
      if(dsps.isEmpty())
      {
        dsps.append(d);
        qDebug()<<"append "<<d;
      }
      else
      {
        bool has=false;
        for(int j=0;j<dsps.size();j++)
        {
          if(d==dsps.at(j))
          {
            has=true;
            break;
          }
        }
        if(has==false)
        {
          dsps.append(d);
          qDebug()<<"append "<<d;
        }
      }
    }

#if CURVE_MANAGER_TEST
  for(int i=0;i<dsps.size();i++)
  {
    qDebug()<<"dsp "<<dsps.at(i);
  }
#endif

    //DSP归类
    QMultiHash<int ,ICurve *> multiHashDSP;
    for(int j=0;j<curves.size();j++)
    {
      int dspInx;
      c=curves.at(j);
      dspInx=c->dspInx();
      multiHashDSP.insert(dspInx,c);
    }

    for(int i=0;i<dsps.size();i++)
    {
      int dspInx =0;
      QList<ICurve *> curvesInDsp;
      dspInx=dsps.at(i);
      curvesInDsp=multiHashDSP.values(dspInx);//单个DSP下面的所有曲线
      qDebug()<<"dspinx"<<dspInx<<"curvesInDsp.size = "<<curvesInDsp.size();

      ComDriver::PlotControlPrm prm;

      prm.dspIndex = dspInx;
      prm.enable = ComDriver::PLOT_DISABLE;
      prm.samplingScale = m_sampleScale;
      prm.number = 0;

      for(int j=0;j<curvesInDsp.size();j++)
      {
        ICurve *c=curvesInDsp.at(j);
        int varSize = c->varInputsKeys().size();
        qDebug()<<"varSize "<<varSize;
        for(int k=0;k<varSize;k++)
        {
          ComDriver::PlotCurveInfo info;
          const CurvePrm *curvePrm=&(c->varInputs().at(k).prm);
          info.base = curvePrm->baseAddr;
          info.bytes = curvePrm->bytes;
          info.ofst = curvePrm->offtAddr;
          prm.curveInfo.push_back(info);
        }
        prm.number = prm.number + varSize;
      }

      devPrm.m_prms.append(prm);
    }

    m_samplPrms.append(devPrm);
  }

#if CURVE_MANAGER_TEST
  for(int i=0;i<m_samplPrms.size();i++)
  {
    const DevSamplePrm *dev=&(m_samplPrms.at(i));
    qDebug()<<"Device = "<<i<<" plot prms is:";
    for(int j=0;j<dev->m_prms.size();j++)
    {
      const ComDriver::PlotControlPrm *prm=&(dev->m_prms.at(j));
      qDebug()<<QString("dspIndex = %1 ,curve number = %2 ,samplScale = %3").arg(prm->dspIndex).arg(prm->number).arg(prm->samplingScale);

      for(int k=0;k<(int)prm->curveInfo.size();k++)
      {
        const ComDriver::PlotCurveInfo *info=&(prm->curveInfo.at(k));
        qDebug()<<QString("base = %1 , bytes = %2 , ofst = %3").arg(info->base).arg(info->bytes).arg(info->ofst);
      }
    }
  }
#endif

  return m_samplPrms;
}

int CurveManager::sampleScale() const
{
  return m_sampleScale;
}

void CurveManager::setSampleScale(int sampleScale)
{
  m_sampleScale = sampleScale;
}

