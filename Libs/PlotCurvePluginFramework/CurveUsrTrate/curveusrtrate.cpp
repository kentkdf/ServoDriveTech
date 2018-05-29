﻿#include "curveusrtrate.h"
#include <QVector>
#include <ctkPluginContext.h>
#include <QDebug>

#define VAR_KEYNAME_CUR_CTL_IQ          "gSevDrv.sev_obj.cur.ctl.iq"
#define CON_KEYNAME_MOT_IMAX            "gSevDrv.sev_obj.cur.mot.Imax_1"
#define CON_KEYNAME_MOT_IRAT            "gSevDrv.sev_obj.cur.mot.Irat_1"

CurveUsrTrate::CurveUsrTrate()
{

}

CurveUsrTrate::CurveUsrTrate(ctkPluginContext *context)
{
  qDebug()<<"CurveUsrTrate construct";
  ctkDictionary dic;
  dic.insert("name","CurveUsrTrate");
  context->registerService<ICurve>(this,dic);
}

ICurve *CurveUsrTrate::clone()
{
  ICurve *c=new CurveUsrTrate();
  c->prepare();
  return c;
}

void CurveUsrTrate::init()
{
  //设置单位
  //增加变量曲线，常量曲线读取 keyName
  addUnit("%",1);
  addVarInputByName(VAR_KEYNAME_CUR_CTL_IQ); //对应输入通道 dd.m_varInputs.at(0).datas
  addConstInputByName(CON_KEYNAME_MOT_IMAX);//对应输入值 dd.m_constInputs.at(0).constK C0
  addConstInputByName(CON_KEYNAME_MOT_IRAT);//对应输入值 dd.m_constInputs.at(1).constK C1
}

void CurveUsrTrate::calculate()
{
  double result = 0;
  double imax = dd.m_constInputs.at(0).constK;
  double irat = dd.m_constInputs.at(1).constK;
//  nos = 5000;
  dd.m_cData.values.clear();
  for(int i = 0;i<dd.m_varInputs.at(0).datas.size();i++)
  {
//    result = (dd.m_varInputs.at(0).datas.at(i)*imax)/(15000.0*irat)*100;
    result = (dd.m_varInputs.at(0).datas.at(i)*imax)/(150.0*irat);
    dd.m_cData.values.append(result);
//    qDebug()<<QString("vcmd %1 cal: in = %2,out = %3").arg(dd.m_axisInx).arg(dd.m_varInputs.at(0).datas.at(i)).arg(result);
  }

}

QString CurveUsrTrate::name()
{
  return QString("Trate");
}

QString CurveUsrTrate::note()
{
  return tr("Torque Rate");
}

QString CurveUsrTrate::pluginName()
{
  return "CurveUsrTrate";
}
