﻿#include "optionmainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

#include "optautoload.h"
#include "iopt.h"
#include "optcontainer.h"
#include "optface.h"
#include "optplot.h"
#include "optuser.h"

#include "gtutils.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  OptContainer *optc=OptContainer::instance();

  IOpt *opt=new OptUser("optuser",0);
  optc->addOptItem(opt);
  opt=new OptAutoLoad("optautoload",0);
  optc->addOptItem(opt);
  opt=new OptPlot("optplot",0);
  optc->addOptItem(opt);
  opt=new OptFace("optface",0);
  optc->addOptItem(opt);

  opt=optc->optItem("optface");
  OptFace *optFace=dynamic_cast<OptFace *>(opt);

  QString langPath=GTUtils::languagePath();
  QString lang;
  if(optFace->language()=="chinese")
  {
    lang=langPath+"ch_main_test.qm";
  }
  else
  {
    lang=langPath+"en_main_test.qm";
  }

  optFace->setFaceStyle(optFace->css());
  optFace->setFaceFontSize(optFace->fontSize());

  qDebug()<<"language "<<lang;
  QTranslator trans;
  trans.load(lang);
  a.installTranslator(&trans);



  OptionMainWindow w;
  w.show();

  return a.exec();
}