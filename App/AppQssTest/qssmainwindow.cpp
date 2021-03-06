﻿#include "qssmainwindow.h"
#include "ui_qssmainwindow.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include "styleiconwidget.h"
#include <QProcess>
#include "Windows.h"

#include "SystemInfo/isysinfo.h"
#include "SystemInfo/syswin.h"


QssMainWindow::QssMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QssMainWindow)
{
  ui->setupUi(this);
  QPushButton *button=new QPushButton(this);
  button->setText(tr("button new by code"));
  ui->verticalLayout->addWidget(button);

  //test StyleIconWidget
  QGridLayout *gridLayout=new QGridLayout();
  ui->verticalLayout->addLayout(gridLayout);
  QString fn="D:\\Smart\\ServoMaster\\git-project\\ServoDriveTech\\build-ServoDriveTech-Qt5_5_1Vs2013x86-Debug\\App\\AppQssTest\\qss\\icon";

  StyleIconWidget *s1=new StyleIconWidget(fn+"\\s1.png","blue","blue",this);
  StyleIconWidget *s2=new StyleIconWidget(fn+"\\s2.png","gray","gray",this);
  StyleIconWidget *s3=new StyleIconWidget(fn+"\\s3.png","red","red",this);
  StyleIconWidget *s4=new StyleIconWidget(fn+"\\s4.png","yellow","yellow",this);
  gridLayout->addWidget(s1,0,0);
  gridLayout->addWidget(s2,0,1);
  gridLayout->addWidget(s3,1,0);
  gridLayout->addWidget(s4,1,1);

  p=new QProcess;
  qDebug()<<"processId"<<p->processId();

  m_sysInfo=new SysWin;
}

QssMainWindow::~QssMainWindow()
{
  delete ui;
}

void QssMainWindow::on_actionQss1_triggered()
{

  QString filename=QApplication::applicationDirPath();
  QDir dir(filename);
  dir.cdUp();
  filename=dir.absolutePath()+"/qss/qss1.css";
  qDebug()<<filename;

  QFile file(filename);
  file.open(QFile::ReadOnly);
  QTextStream in(&file);

  QString qss = in.readAll();
  qApp->setStyleSheet(qss);
  qDebug()<<"processId"<<p->processId();

}

void QssMainWindow::on_actionQss2_triggered()
{
  QString filename=QApplication::applicationDirPath();
  QDir dir(filename);
  dir.cdUp();
  filename=dir.absolutePath()+"/qss/qss2.css";
  qDebug()<<filename;
  QFile file(filename);
  file.open(QFile::ReadOnly);
  QTextStream in(&file);

  QString qss = in.readAll();
  qApp->setStyleSheet(qss);
}

void QssMainWindow::on_actionQ3_triggered()
{
  QString qss="QPushButton{\
      border-style: none;\
      border: 0px;\
      color: #F0F0F0;\
      padding: 5px;\
      min-height: 20px;\
      border-radius:5px;\
      background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #4D4D4D, stop:1 #292929);\
    }";
qApp->setStyleSheet(qss);
}

void QssMainWindow::on_pushButton_fontsize_clicked()
{
  QFont font=qApp->font();
  font.setPixelSize(ui->spinBox->value());
  qApp->setFont(font);
  //还要再设置一下样式，才能更新
}

void QssMainWindow::on_pushButton_clicked()
{
  QList<QWidget *>strList;
  QWidget *stest;
  for(int i=0;i<1000;i++)
  {
    stest=new QWidget;
    stest->show();
//    testCupInfo();
    Sleep(10);
    strList.append(stest);
  }
  qDebug()<<"release **********************";
  for(int j=0;j<strList.count();j++)
  {
    delete strList.at(j);
//    testCupInfo();
  }
  strList.clear();
  qDebug()<<"clicked";
}

void QssMainWindow::testCupInfo()
{
  int nCpuRate = -1;
//  int nMemTotal = -1;
//  int nMemUsed = -1;
//  int nDiskTotal = -1;
//  int nDiskUsed = -1;
//  int nProcessMemRate = -1;
//  QMap<int,QString> pidMap;

  m_sysInfo->GetSysCpu(nCpuRate);
  qDebug()<<"CPU Rate:"<<nCpuRate<<"%";

//  m_sysInfo->GetSysMemory(nMemTotal,nMemUsed);
//  qDebug()<<"Mem Total:"<<nMemTotal<<"\t Mem Used:"<<nMemUsed;

//  m_sysInfo->GetSysDisk(nDiskTotal,nDiskUsed);
//  qDebug()<<"Disk Total:"<<(nDiskTotal/1024.0)<<"GB \t Disk Used:"<<(nDiskUsed/1024.0)<<"GB";

//  m_sysInfo->GetProcessMemory(6472,nProcessMemRate);
//  qDebug()<<"PID:6472;\t Mem Rate:"<<nProcessMemRate<<"%";

//  pidMap = m_sysInfo->GetAllProcess();
//  qDebug()<<"Process Number:"<<pidMap.size();

  qDebug()<<"-------------------------------------------------------------------";
}
