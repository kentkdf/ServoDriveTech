﻿#include "widgetmainwindow.h"
#include "formmain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  FormMain w;
  w.show();

  return a.exec();
}