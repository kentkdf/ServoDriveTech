﻿#include "qssmainwindow.h"
#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QssMainWindow w;
  w.show();

  QThread::currentThread()->setPriority(QThread::LowestPriority);

  return a.exec();
}
