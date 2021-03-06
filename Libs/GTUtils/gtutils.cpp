﻿#include "gtutils.h"
#include <QApplication>
#include <QDir>
#include <QTime>
#include <QTreeWidget>

//这些路径都是相对debug/release目录下
#define DIR_COMMON "common/"
#define DIR_CUSTOM "custom/"
#define DIR_RESOURCE "resource/"
#define DIR_SYS "sysconfig/"
#define DIR_DATABASE "resource/database/"
#define DIR_ICON "resource/icon/"
#define DIR_UBOOT "resource/uboot/"
#define DIR_LANGUAGE "resource/language/"


GTUtils::GTUtils()
{
}
QString GTUtils::exePath()
{
  return QApplication::applicationDirPath();
}

QString GTUtils::commonPath()
{
  QString path=sdtPath()+DIR_COMMON;
  return path;
}

QString GTUtils::customPath()
{
  QString path=sdtPath()+DIR_CUSTOM;
  return path;
}

QString GTUtils::resPath()
{
  QString path=sdtPath()+DIR_RESOURCE;
  return path;
}

QString GTUtils::sysPath()
{
  QString path=sdtPath()+DIR_SYS;
  return path;
}
QString GTUtils::databasePath()
{
  QString path=sdtPath()+DIR_DATABASE;
  return path;
}
QString GTUtils::iconPath()
{
  QString path=sdtPath()+DIR_ICON;
  return path;
}

QString GTUtils::ubootPath()
{
  QString path=sdtPath()+DIR_UBOOT;
  return path;
}

QString GTUtils::languagePath()
{
  QString path=sdtPath()+DIR_LANGUAGE;
  return path;
}

void GTUtils::delayms(quint16 ms)
{
  QTime dieTime = QTime::currentTime().addMSecs(ms);
  while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void GTUtils::clearTreeWidgetList(QList<QTreeWidget *> &list)
{
  QTreeWidget *tree;
  for(int i=0;i<list.count();i++)
  {
    tree=list.at(i);
    delete tree;
  }
  list.clear();
}

//--------------------private function-------------------------------
QString GTUtils::sdtPath()
{
  QString path=QApplication::applicationDirPath();
  QDir dir(path);
  dir.cdUp();
  path=dir.absolutePath()+"/";
  return path;
}


